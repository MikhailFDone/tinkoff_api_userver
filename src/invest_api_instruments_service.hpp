#pragma once

#include <invest_api/instruments_client.usrv.pb.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/impl/component_base.hpp>
#include "invest_api_service_common.hpp"

namespace tinkoff_api_userver
{

namespace invest_api = tinkoff::public_::invest::api::contract::v1;

#define INVEST_API_METHOD_WITH_INSTRUMENTS_REQUEST_IMPL(method_name) \
	const invest_api::method_name##Response method_name##Impl() \
	{ \
		invest_api::InstrumentsRequest request; \
		auto stream = client_.method_name(request, CreateGrpcContext()); \
		invest_api::method_name##Response response; \
		response = stream.Finish(); \
		return response; \
	}

class InvestApiInstrumentsClient final
    : public InvestApiServiceCommon {
public:
	static constexpr std::string_view kName = "invest-api-instruments-client";

	InvestApiInstrumentsClient(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : InvestApiServiceCommon(config, component_context),
        client_factory_(
            component_context
                .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
                .GetFactory()),
        // The client needs a fixed endpoint
        client_(
            client_factory_.MakeClient<invest_api::InstrumentsServiceClient>(
                config["endpoint"].As<std::string>())),
		invest_api_token_(config["token"].As<std::string>())
		{}

	INVEST_API_METHOD_IMPL(TradingSchedules)
	INVEST_API_METHOD_CUSTOM_IMPL(BondBy, InstrumentRequest, BondResponse)
	INVEST_API_METHOD_WITH_INSTRUMENTS_REQUEST_IMPL(Bonds)
	INVEST_API_METHOD_IMPL(GetBondCoupons)
	INVEST_API_METHOD_CUSTOM_IMPL(CurrencyBy, InstrumentRequest, CurrencyResponse)
	INVEST_API_METHOD_WITH_INSTRUMENTS_REQUEST_IMPL(Currencies)
	//INVEST_API_METHOD_IMPL(EtfBy)
	INVEST_API_METHOD_WITH_INSTRUMENTS_REQUEST_IMPL(Etfs)
	//INVEST_API_METHOD_IMPL(FutureBy)
	INVEST_API_METHOD_WITH_INSTRUMENTS_REQUEST_IMPL(Futures)
	//INVEST_API_METHOD_IMPL(OptionBy)
	INVEST_API_METHOD_WITH_INSTRUMENTS_REQUEST_IMPL(Options)
	INVEST_API_METHOD_CUSTOM_IMPL(ShareBy, InstrumentRequest, ShareResponse)
	INVEST_API_METHOD_WITH_INSTRUMENTS_REQUEST_IMPL(Shares)
	INVEST_API_METHOD_IMPL(GetAccruedInterests)
	INVEST_API_METHOD_IMPL(GetFuturesMargin)
	INVEST_API_METHOD_CUSTOM_IMPL(GetInstrumentBy, InstrumentRequest, InstrumentResponse)
	INVEST_API_METHOD_IMPL(GetDividends)
	INVEST_API_METHOD_CUSTOM_IMPL(GetAssetBy, AssetRequest, AssetResponse)
	INVEST_API_METHOD_CUSTOM_IMPL(GetAssets, AssetsRequest, AssetsResponse)
	INVEST_API_METHOD(GetFavorites)
	INVEST_API_METHOD_IMPL(EditFavorites)
	INVEST_API_METHOD_IMPL(GetCountries)
	INVEST_API_METHOD_IMPL(FindInstrument)
	INVEST_API_METHOD_IMPL(GetBrands)
	//INVEST_API_METHOD_IMPL(GetBrandBy)


	invest_api::InstrumentResponse GetInstrumentBy(invest_api::InstrumentIdType instrument_id_type, std::string_view id, std::string_view class_code = "")
	{
		UASSERT((instrument_id_type == invest_api::InstrumentIdType::INSTRUMENT_ID_TYPE_TICKER) == !class_code.empty());
		
		invest_api::InstrumentRequest request;

		request.set_id_type(instrument_id_type);
		request.set_class_code(class_code.data(), class_code.size());
		request.set_id(id.data(), id.size());

		return GetInstrumentByImpl(request);
	}

	invest_api::BondResponse BondBy(invest_api::InstrumentIdType instrument_id_type, std::string_view id, std::string_view class_code = "")
	{
		UASSERT((instrument_id_type == invest_api::InstrumentIdType::INSTRUMENT_ID_TYPE_TICKER) == !class_code.empty());

		invest_api::InstrumentRequest request;

		request.set_id_type(instrument_id_type);
		request.set_class_code(class_code.data(), class_code.size());
		request.set_id(id.data(), id.size());

		return BondByImpl(request);
	}

	invest_api::GetBondCouponsResponse GetBondCoupons(std::string_view figi, Timestamp from, Timestamp to)
	{
		invest_api::GetBondCouponsRequest request;

		request.set_figi(figi.data(), figi.size());
		request.mutable_from()->CopyFrom(from);
		request.mutable_to()->CopyFrom(to);

		return GetBondCouponsImpl(request);
	}

	static userver::yaml_config::Schema GetStaticConfigSchema();

private:

	userver::ugrpc::client::ClientFactory& client_factory_;
	invest_api::InstrumentsServiceClient client_;
	std::string invest_api_token_;
};

void AppendInvestApiInstrumentsClient(
    userver::components::ComponentList& component_list);

}  // namespace tinkoff_api_userver

template <>
inline constexpr bool userver::components::kHasValidate<
    tinkoff_api_userver::InvestApiInstrumentsClient> = true;
