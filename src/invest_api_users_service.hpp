#pragma once

#include <invest_api/users_client.usrv.pb.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/impl/component_base.hpp>
#include "invest_api_service_common.hpp"

namespace tinkoff_api_userver
{

namespace invest_api = tinkoff::public_::invest::api::contract::v1;

class InvestApiUsersClient final
    : public InvestApiServiceCommon {
public:
	static constexpr std::string_view kName = "invest-api-users-client";

	InvestApiUsersClient(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : InvestApiServiceCommon(config, component_context),
        client_factory_(
            component_context
                .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
                .GetFactory()),
        // The client needs a fixed endpoint
        client_(
            client_factory_.MakeClient<invest_api::UsersServiceClient>(
                config["endpoint"].As<std::string>())),
		invest_api_token_(config["token"].As<std::string>())
		{}

	INVEST_API_METHOD(GetAccounts)
	INVEST_API_METHOD(GetInfo)
	INVEST_API_METHOD_IMPL(GetMarginAttributes)
	INVEST_API_METHOD(GetUserTariff)

	static userver::yaml_config::Schema GetStaticConfigSchema();

private:
	userver::ugrpc::client::ClientFactory& client_factory_;
	invest_api::UsersServiceClient client_;
	std::string invest_api_token_;
};

void AppendInvestApiUsersClient(
    userver::components::ComponentList& component_list);

}  // namespace tinkoff_api_userver

template <>
inline constexpr bool userver::components::kHasValidate<
    tinkoff_api_userver::InvestApiUsersClient> = true;
