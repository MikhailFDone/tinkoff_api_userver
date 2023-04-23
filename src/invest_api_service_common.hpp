#pragma once

#include <userver/components/component_list.hpp>
#include <userver/components/impl/component_base.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>
#include <invest_api/common.pb.h>

namespace tinkoff_api_userver
{

namespace invest_api = tinkoff::public_::invest::api::contract::v1;

#define INVEST_API_METHOD(method_name) \
	const invest_api::method_name##Response method_name() \
	{ \
		invest_api::method_name##Request request; \
		auto stream = client_.method_name(request, CreateGrpcContext()); \
		invest_api::method_name##Response response; \
		response = stream.Finish(); \
		return response; \
	}

#define INVEST_API_METHOD_IMPL(method_name) \
	const invest_api::method_name##Response method_name##Impl() \
	{ \
		invest_api::method_name##Request request; \
		auto stream = client_.method_name(request, CreateGrpcContext()); \
		invest_api::method_name##Response response; \
		response = stream.Finish(); \
		return response; \
	}

#define INVEST_API_METHOD_CUSTOM_IMPL(method_name, request_type, response_type) \
	const invest_api::response_type method_name##Impl(const invest_api::request_type& request) \
	{ \
		auto stream = client_.method_name(request, CreateGrpcContext()); \
		invest_api::response_type response; \
		response = stream.Finish(); \
		return response; \
	}

class InvestApiServiceCommon
    : public userver::components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "invest-api-service-common";

	InvestApiServiceCommon(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : userver::components::LoggableComponentBase(config, component_context),
		endpoint_(config["endpoint"].As<std::string>()),
		invest_api_token_(config["token"].As<std::string>())
		{}

	static userver::yaml_config::Schema GetStaticConfigSchema();

protected:
	std::unique_ptr<grpc::ClientContext> CreateGrpcContext() const;

private:
	std::string endpoint_;
	std::string invest_api_token_;
};

void AppendInvestApiServiceCommon(
    userver::components::ComponentList& component_list);

}  // namespace tinkoff_api_userver

template <>
inline constexpr bool userver::components::kHasValidate<
    tinkoff_api_userver::InvestApiServiceCommon> = true;
