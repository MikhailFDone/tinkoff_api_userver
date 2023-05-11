#pragma once

#include <invest_api/marketdata_client.usrv.pb.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/impl/component_base.hpp>
#include "invest_api_service_common.hpp"

namespace tinkoff_api_userver
{

namespace invest_api = tinkoff::public_::invest::api::contract::v1;

class InvestApiMarketdataClient final
    : public InvestApiServiceCommon {
public:
	static constexpr std::string_view kName = "invest-api-marketdata-client";

	InvestApiMarketdataClient(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : InvestApiServiceCommon(config, component_context),
        client_factory_(
            component_context
                .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
                .GetFactory()),
        // The client needs a fixed endpoint
        client_(
            client_factory_.MakeClient<invest_api::MarketDataServiceClient>(
                config["endpoint"].As<std::string>())),
		invest_api_token_(config["token"].As<std::string>())
		{}

	static userver::yaml_config::Schema GetStaticConfigSchema();

private:
	userver::ugrpc::client::ClientFactory& client_factory_;
	invest_api::MarketDataServiceClient client_;
	std::string invest_api_token_;
};

void AppendInvestApiMarketdataClient(
    userver::components::ComponentList& component_list);

class InvestApiMarketdataStreamClient final
    : public InvestApiServiceCommon {
public:
	static constexpr std::string_view kName = "invest-api-marketdata-stream-client";

	InvestApiMarketdataStreamClient(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : InvestApiServiceCommon(config, component_context),
        client_factory_(
            component_context
                .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
                .GetFactory()),
        // The client needs a fixed endpoint
        client_(
            client_factory_.MakeClient<invest_api::MarketDataStreamServiceClient>(
                config["endpoint"].As<std::string>())),
		invest_api_token_(config["token"].As<std::string>())
		{}

    invest_api::MarketDataStreamServiceClient::MarketDataStreamCall MarketDataStream(invest_api::MarketDataRequest&& request)
    {
        auto bidirectional_stream = client_.MarketDataStream(CreateGrpcContext());
        
        bidirectional_stream.WriteAndCheck(request);

        return bidirectional_stream;
    }

    invest_api::MarketDataStreamServiceClient::MarketDataStreamCall SubscribeCandlesRequest(
        const std::vector<invest_api::CandleInstrument>& instruments,
        invest_api::SubscriptionAction subscription_action = invest_api::SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE,
        bool waiting_close = true)
    {
        auto request = new invest_api::SubscribeCandlesRequest;
        
        for (const auto& instrument : instruments)
        {
            request->add_instruments()->CopyFrom(instrument);
        }
        request->set_subscription_action(subscription_action);
        request->set_waiting_close(waiting_close);

        invest_api::MarketDataRequest md_request;
        md_request.set_allocated_subscribe_candles_request(request);

        auto stream = MarketDataStream(std::move(md_request));
        return stream;
    }

	static userver::yaml_config::Schema GetStaticConfigSchema();

private:
	userver::ugrpc::client::ClientFactory& client_factory_;
	invest_api::MarketDataStreamServiceClient client_;
	std::string invest_api_token_;
};

void AppendInvestApiMarketdataStreamClient(
    userver::components::ComponentList& component_list);

}  // namespace tinkoff_api_userver

template <>
inline constexpr bool userver::components::kHasValidate<
    tinkoff_api_userver::InvestApiMarketdataClient> = true;

template <>
inline constexpr bool userver::components::kHasValidate<
    tinkoff_api_userver::InvestApiMarketdataStreamClient> = true;