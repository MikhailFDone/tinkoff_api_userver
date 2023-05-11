#include "invest_api_marketdata_service.hpp"

#include <fmt/format.h>

#include <userver/yaml_config/merge_schemas.hpp>

namespace tinkoff_api_userver {

userver::yaml_config::Schema InvestApiMarketdataClient::GetStaticConfigSchema() {
  auto schema = InvestApiServiceCommon::GetStaticConfigSchema();
  schema.UpdateDescription(
      "MarketdataService component, it depends on "
      "InvestApiServiceCommon.");
  return schema;
}

void AppendInvestApiMarketdataClient(userver::components::ComponentList& component_list) {
  component_list.Append<InvestApiMarketdataClient>();
}

userver::yaml_config::Schema InvestApiMarketdataStreamClient::GetStaticConfigSchema() {
  auto schema = InvestApiServiceCommon::GetStaticConfigSchema();
  schema.UpdateDescription(
      "MarketdataStreamService component, it depends on "
      "InvestApiServiceCommon.");
  return schema;
}

void AppendInvestApiMarketdataStreamClient(userver::components::ComponentList& component_list) {
  component_list.Append<InvestApiMarketdataStreamClient>();
}

}  // namespace tinkoff_api_userver
