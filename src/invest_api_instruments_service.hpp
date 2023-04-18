#pragma once

#include <invest_api/instruments_client.usrv.pb.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/impl/component_base.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>

namespace tinkoff_api_userver
{

namespace invest_api = tinkoff::public_::invest::api::contract::v1;

#define INVEST_API_METHOD_IMPL(method_name) \
	const invest_api::method_name##Response method_name() \
	{ \
		invest_api::method_name##Request request; \
		auto stream = client_.method_name(request, CreateGrpcContext()); \
		invest_api::method_name##Response response; \
		response = stream.Finish(); \
		return response; \
	}

#define INVEST_API_METHOD_INSTRUMENTS_IMPL(method_name) \
	const invest_api::method_name##Response method_name() \
	{ \
		invest_api::InstrumentsRequest request; \
		auto stream = client_.method_name(request, CreateGrpcContext()); \
		invest_api::method_name##Response response; \
		response = stream.Finish(); \
		return response; \
	}

class InvestApiInstrumentsClient final
    : public userver::components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "invest-api-instruments-client";

	InvestApiInstrumentsClient(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : userver::components::LoggableComponentBase(config, component_context),
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
	//INVEST_API_METHOD_IMPL(BondBy)
	INVEST_API_METHOD_INSTRUMENTS_IMPL(Bonds)
	INVEST_API_METHOD_IMPL(GetBondCoupons)
	//INVEST_API_METHOD_IMPL(CurrencyBy)
	INVEST_API_METHOD_INSTRUMENTS_IMPL(Currencies)
	//INVEST_API_METHOD_IMPL(EtfBy)
	INVEST_API_METHOD_INSTRUMENTS_IMPL(Etfs)
	//INVEST_API_METHOD_IMPL(FutureBy)
	INVEST_API_METHOD_INSTRUMENTS_IMPL(Futures)
	//INVEST_API_METHOD_IMPL(OptionBy)
	INVEST_API_METHOD_INSTRUMENTS_IMPL(Options)
	//INVEST_API_METHOD_IMPL(ShareBy)
	INVEST_API_METHOD_INSTRUMENTS_IMPL(Shares)
	INVEST_API_METHOD_IMPL(GetAccruedInterests)
	INVEST_API_METHOD_IMPL(GetFuturesMargin)
	//INVEST_API_METHOD_IMPL(GetInstrumentBy)
	INVEST_API_METHOD_IMPL(GetDividends)
	//INVEST_API_METHOD_IMPL(GetAssetBy)
	//INVEST_API_METHOD_IMPL(GetAssets)
	INVEST_API_METHOD_IMPL(GetFavorites)
	INVEST_API_METHOD_IMPL(EditFavorites)
	INVEST_API_METHOD_IMPL(GetCountries)
	INVEST_API_METHOD_IMPL(FindInstrument)
	INVEST_API_METHOD_IMPL(GetBrands)
	//INVEST_API_METHOD_IMPL(GetBrandBy)

	static userver::yaml_config::Schema GetStaticConfigSchema();

private:
	std::unique_ptr<grpc::ClientContext> CreateGrpcContext() const;

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
