#include "strategy_stream.hpp"

#include <fmt/format.h>

#include <userver/clients/dns/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/assert.hpp>
#include <userver/utils/async.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <date/date.h>
#include <google/protobuf/util/time_util.h>

namespace tinkoff_api_userver {

void StrategyStream::OnAllComponentsLoaded() {
    std::vector<invest_api::CandleInstrument> instruments;
    instruments.resize(1);
    instruments.front().set_interval(invest_api::SubscriptionInterval::SUBSCRIPTION_INTERVAL_ONE_MINUTE);
    instruments.front().set_instrument_id("BBG004730N88");
    auto stream = invest_api_marketdata_stream_.SubscribeCandlesRequest(instruments);

    invest_api::MarketDataResponse md_response;
    bool result = stream.Read(md_response);

    UASSERT(result); // не ожидаем, что стрим сразу закроется
    UASSERT(md_response.has_subscribe_candles_response());
    UASSERT(!md_response.has_candle()); // ожидаем только результат подписки в первом сообщении

    std::cout << "tracking_id: " << md_response.subscribe_candles_response().tracking_id() << std::endl;
    for (const auto& subscriptions : md_response.subscribe_candles_response().candles_subscriptions())
    {
        std::cout << subscriptions.figi() << " " << invest_api::SubscriptionStatus_Name(subscriptions.subscription_status()) << std::endl;
    }

    tasks_.emplace(userver::utils::AsyncBackground("OnNewCandle", task_processor_, std::bind(&StrategyStream::OnNewCandle, this, std::placeholders::_1), std::move(stream)));
}

void StrategyStream::OnNewCandle(invest_api::MarketDataStreamServiceClient::MarketDataStreamCall&& stream)
{
    if (tasks_.size() > 1)
    {
        tasks_.pop();
    }

    invest_api::MarketDataResponse md_response;
    bool result = stream.Read(md_response);

    UASSERT(result); // не ожидаем, что стрим закроется

    std::cout << md_response.candle().DebugString() << std::endl;
    std::cout << md_response.ping().DebugString() << std::endl;

    tasks_.emplace(userver::utils::AsyncBackground("OnNewCandle", task_processor_, std::bind(&StrategyStream::OnNewCandle, this, std::placeholders::_1), std::move(stream)));
}

userver::yaml_config::Schema StrategyStream::GetStaticConfigSchema() {
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
    task-processor:
        type: string
        description: >
            ikiki-ololo
)");
}

void AppendStrategyStream(userver::components::ComponentList& component_list) {
	component_list.Append<StrategyStream>();
}

}  // namespace tinkoff_api_userver
