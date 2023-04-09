#pragma once

#include <userver/components/component_list.hpp>
#include <userver/components/impl/component_base.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>
#include "invest_api_users_service.hpp"

namespace tinkoff_api_userver {

class Strategy final : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "strategy";

  Strategy(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_context)
      : userver::components::LoggableComponentBase(config, component_context),
        client_factory_(
            component_context
                .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
                .GetFactory()),
        invest_api_users_(
            component_context.FindComponent<InvestApiUsersClient>()) {}

  void OnAllComponentsLoaded() override;

  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  userver::ugrpc::client::ClientFactory& client_factory_;
  tinkoff_api_userver::InvestApiUsersClient& invest_api_users_;
};

void AppendStrategy(userver::components::ComponentList& component_list);

}  // namespace tinkoff_api_userver

template <>
inline constexpr bool
    userver::components::kHasValidate<tinkoff_api_userver::Strategy> = true;
