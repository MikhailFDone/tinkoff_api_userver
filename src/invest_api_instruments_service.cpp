#include "invest_api_instruments_service.hpp"

#include <fmt/format.h>

#include <userver/yaml_config/merge_schemas.hpp>

namespace tinkoff_api_userver {

userver::yaml_config::Schema InvestApiInstrumentsClient::GetStaticConfigSchema() {
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

void AppendInvestApiInstrumentsClient(userver::components::ComponentList& component_list) {
  component_list.Append<InvestApiInstrumentsClient>();
}

}  // namespace tinkoff_api_userver
