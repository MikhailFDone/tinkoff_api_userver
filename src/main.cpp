#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>
#include <userver/ugrpc/server/server_component.hpp>
#include <userver/utils/daemon_run.hpp>

#include "hello.hpp"
#include "hello_client.hpp"
#include "invest_api_users_service.hpp"
#include "invest_api_instruments_service.hpp"
#include "strategy.hpp"

int main(int argc, char* argv[]) {
  auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::ugrpc::server::ServerComponent>()
          .Append<userver::ugrpc::client::ClientFactoryComponent>()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::components::HttpClient>()
          .Append<userver::server::handlers::TestsControl>();

  tinkoff_api_userver::AppendHello(component_list);
  tinkoff_api_userver::AppendHelloClient(component_list);
  tinkoff_api_userver::AppendInvestApiUsersClient(component_list);
  tinkoff_api_userver::AppendInvestApiInstrumentsClient(component_list);
  tinkoff_api_userver::AppendStrategy(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
