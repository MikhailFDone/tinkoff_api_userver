#include "strategy.hpp"

#include <fmt/format.h>

#include <userver/clients/dns/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/assert.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

namespace tinkoff_api_userver {

void Strategy::OnAllComponentsLoaded() {
  std::cout << invest_api_users_.GetInfo().DebugString() << std::endl;
  std::cout << invest_api_users_.GetAccounts().accounts(0).name() << std::endl;
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
