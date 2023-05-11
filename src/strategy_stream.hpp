#pragma once

#include <userver/components/component_list.hpp>
#include <userver/components/impl/component_base.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>
#include <userver/engine/task/task_processor_fwd.hpp>
#include "invest_api_marketdata_service.hpp"

#include <queue>

namespace tinkoff_api_userver {

class StrategyStream final : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "strategy-stream";

  StrategyStream(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_context)
      : userver::components::LoggableComponentBase(config, component_context),
        client_factory_(
            component_context
                .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
                .GetFactory()),
        invest_api_marketdata_stream_(
            component_context.FindComponent<InvestApiMarketdataStreamClient>()),
        task_processor_(
            component_context.GetTaskProcessor(config["task-processor"].As<std::string>()))
      {}

  void OnAllComponentsLoaded() override;

  void OnNewCandle(invest_api::MarketDataStreamServiceClient::MarketDataStreamCall&& stream);

  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  userver::ugrpc::client::ClientFactory& client_factory_;
  tinkoff_api_userver::InvestApiMarketdataStreamClient& invest_api_marketdata_stream_;
  userver::engine::TaskProcessor& task_processor_;
  std::queue<userver::engine::Task> tasks_;
};

void AppendStrategyStream(userver::components::ComponentList& component_list);

}  // namespace tinkoff_api_userver

template <>
inline constexpr bool
    userver::components::kHasValidate<tinkoff_api_userver::StrategyStream> = true;
