#include "invest_api_operations_service.hpp"

#include <fmt/format.h>

#include <userver/yaml_config/merge_schemas.hpp>

namespace tinkoff_api_userver {

userver::yaml_config::Schema InvestApiOperationsClient::GetStaticConfigSchema() {
  auto schema = InvestApiServiceCommon::GetStaticConfigSchema();
  schema.UpdateDescription(
      "OperationsService component, it depends on "
      "InvestApiServiceCommon.");
  return schema;
}

void AppendInvestApiOperationsClient(userver::components::ComponentList& component_list) {
  component_list.Append<InvestApiOperationsClient>();
}

}  // namespace tinkoff_api_userver
