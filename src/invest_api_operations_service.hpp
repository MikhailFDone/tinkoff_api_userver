#pragma once

#include <invest_api/operations_client.usrv.pb.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/impl/component_base.hpp>
#include "invest_api_service_common.hpp"

namespace tinkoff_api_userver
{

namespace invest_api = tinkoff::public_::invest::api::contract::v1;

class InvestApiOperationsClient final
    : public InvestApiServiceCommon {
public:
	static constexpr std::string_view kName = "invest-api-operations-client";

	InvestApiOperationsClient(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : InvestApiServiceCommon(config, component_context),
        client_factory_(
            component_context
                .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
                .GetFactory()),
        // The client needs a fixed endpoint
        client_(
            client_factory_.MakeClient<invest_api::OperationsServiceClient>(
                config["endpoint"].As<std::string>())),
		invest_api_token_(config["token"].As<std::string>())
		{}

    //INVEST_API_METHOD_IMPL(GetOperations)
    INVEST_API_METHOD_CUSTOM_IMPL(GetPortfolio, PortfolioRequest, PortfolioResponse)
    //INVEST_API_METHOD_IMPL(GetPositions)
    //INVEST_API_METHOD_IMPL(GetWithdrawLimits)
    //INVEST_API_METHOD_IMPL(GetBrokerReport)
    //INVEST_API_METHOD_IMPL(GetDividendsForeignIssuer)
    //INVEST_API_METHOD_IMPL(GetOperationsByCursor)

    invest_api::PortfolioResponse GetPortfolio(std::string_view account_id, invest_api::PortfolioRequest::CurrencyRequest currency = invest_api::PortfolioRequest_CurrencyRequest_RUB)
	{
		invest_api::PortfolioRequest request;

		request.set_account_id(account_id.data(), account_id.size());
		request.set_currency(currency);

		return GetPortfolioImpl(request);
	}

	static userver::yaml_config::Schema GetStaticConfigSchema();

private:
	userver::ugrpc::client::ClientFactory& client_factory_;
	invest_api::OperationsServiceClient client_;
	std::string invest_api_token_;
};

void AppendInvestApiOperationsClient(
    userver::components::ComponentList& component_list);

}  // namespace tinkoff_api_userver

template <>
inline constexpr bool userver::components::kHasValidate<
    tinkoff_api_userver::InvestApiOperationsClient> = true;
