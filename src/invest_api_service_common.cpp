#include "invest_api_service_common.hpp"

#include <fmt/format.h>

#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/ugrpc/client/impl/codegen_declarations.hpp>

namespace tinkoff_api_userver {

std::unique_ptr<grpc::ClientContext> InvestApiServiceCommon::CreateGrpcContext() const
{
	// Deadline must be set manually for each RPC
	auto context = std::make_unique<grpc::ClientContext>();
	context->AddMetadata("authorization", "Bearer " + invest_api_token_);
	context->set_deadline(userver::engine::Deadline::FromDuration(std::chrono::seconds{600}));
	context->set_wait_for_ready(true);

	return context;
}

userver::yaml_config::Schema InvestApiServiceCommon::GetStaticConfigSchema() {
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

void AppendInvestApiServiceCommon(userver::components::ComponentList& component_list) {
  component_list.Append<InvestApiServiceCommon>();
}

}  // namespace tinkoff_api_userver
