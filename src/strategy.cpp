#include "strategy.hpp"

#include <fmt/format.h>

#include <userver/clients/dns/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/assert.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <date/date.h>
#include <google/protobuf/util/time_util.h>

namespace tinkoff_api_userver {

void Strategy::OnAllComponentsLoaded() {
  std::cout << invest_api_users_.GetInfo().DebugString() << std::endl;
  std::cout << invest_api_users_.GetAccounts().accounts(0).name() << std::endl;
  std::cout << invest_api_instruments_.GetFavorites().favorite_instruments(0).figi() << std::endl;

  auto today = date::year_month_day{date::floor<date::days>(std::chrono::system_clock::now())};
  auto next_month_date = today + date::months{1};  
  std::cout << today << next_month_date << std::endl;
  
  // TODO: compare time_since_epoch and google::protobuf::util::TimeUtil::GetEpoch()

  auto today_ts = google::protobuf::util::TimeUtil::NanosecondsToTimestamp(
    std::chrono::nanoseconds{date::sys_days(today).time_since_epoch()}.count()
  );
  auto next_month_date_ts = google::protobuf::util::TimeUtil::NanosecondsToTimestamp(
    std::chrono::nanoseconds{date::sys_days(next_month_date).time_since_epoch()}.count()
  );

  auto my_accounts = invest_api_users_.GetAccounts().accounts();
  for (const auto& account : my_accounts)
  {
    std::vector<std::string> bonds_figi;

    auto positions = invest_api_operations_.GetPortfolio(account.id()).positions();
    for (const auto& instrument : positions)
    {
        if (instrument.instrument_type() == "bond")
        {
            bonds_figi.push_back(instrument.figi());
        }
    }

    if (bonds_figi.empty())
        continue;
    
    for (const auto& bond_figi : bonds_figi)
    {
        auto bond = invest_api_instruments_.BondBy(invest_api::InstrumentIdType::INSTRUMENT_ID_TYPE_FIGI, bond_figi, "");
        auto bond_coupons_info = invest_api_instruments_.GetBondCoupons(bond_figi, today_ts, next_month_date_ts);
        
        std::cout << bond.DebugString() << std::endl;
        std::cout << bond_coupons_info.DebugString() << std::endl;

    }

   /*
   for bond_figi in bonds_figi:
                bond = client.instruments.bond_by(id_type=t_invest.InstrumentIdType.INSTRUMENT_ID_TYPE_FIGI, id=bond_figi)
                bond_coupons_info = client.instruments.get_bond_coupons(figi=bond_figi, from_=today, to=next_month_date)
                
                quantity = t_invest.Quotation()

                for instrument_pos in portfolio.positions:
                    if instrument_pos.instrument_type == "bond" and instrument_pos.figi == bond_figi:
                        quantity = instrument_pos.quantity.units

                for bond_coupon_info in bond_coupons_info.events:
                    bond_coupons.append(BondCoupon(bond_figi, bond.instrument.name, bond_coupon_info.coupon_date.date(), bond_coupon_info.pay_one_bond, quantity))
   */
  }  
}

userver::yaml_config::Schema Strategy::GetStaticConfigSchema() {
	return userver::yaml_config::MergeSchemas<
		userver::components::LoggableComponentBase>(R"(
type: object
description: >
    a user-defined wrapper around api::GreeterServiceClient that provides
    a simplified interface.
additionalProperties: false
properties:
    endpoint:
        type: string
        description: >
            Some other service endpoint (URI). 
)");
}

void AppendStrategy(userver::components::ComponentList& component_list) {
	component_list.Append<Strategy>();
}

}  // namespace tinkoff_api_userver
