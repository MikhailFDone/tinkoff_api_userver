#include "invest_api_users_service.hpp"

#include <fmt/format.h>

#include <userver/yaml_config/merge_schemas.hpp>

namespace tinkoff_api_userver {

userver::yaml_config::Schema InvestApiUsersClient::GetStaticConfigSchema() {
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
          endpoint
    token:
        type: string
        description: >
          token
)");
}

void AppendInvestApiUsersClient(userver::components::ComponentList& component_list) {
  component_list.Append<InvestApiUsersClient>();
}

}  // namespace tinkoff_api_userver
