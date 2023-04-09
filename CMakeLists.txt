cmake_minimum_required(VERSION 3.12)
project(tinkoff_api_userver CXX)

set(USERVER_FEATURE_CRYPTOPP_BLAKE2 0)

include(third_party/userver/cmake/SetupEnvironment.cmake)
include(GNUInstallDirs)

add_subdirectory(third_party/userver)


# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
    src/hello.hpp
    src/hello.cpp
    src/hello_client.hpp
    src/hello_client.cpp
    src/invest_api_users_service.hpp
    src/invest_api_users_service.cpp
    src/strategy.hpp
    src/strategy.cpp
)
# Grpc enable
include(GrpcTargets)

add_grpc_library(${PROJECT_NAME}_proto PROTOS handlers/hello.proto)

add_grpc_library(${PROJECT_NAME}_api_proto 
    SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/proto"
    INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/proto/invest_api"
    PROTOS
    invest_api/common.proto
    invest_api/instruments.proto
    invest_api/marketdata.proto
    invest_api/operations.proto
    invest_api/orders.proto
    invest_api/sandbox.proto
    invest_api/stoporders.proto
    invest_api/users.proto
)

target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver-core userver-postgresql ${PROJECT_NAME}_proto ${PROJECT_NAME}_api_proto)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)


# Unit Tests
add_executable(${PROJECT_NAME}_unittest
    src/hello_test.cpp
)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver-utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Benchmarks
add_executable(${PROJECT_NAME}_benchmark
	src/hello_benchmark.cpp
)
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver-ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)

# Functional Tests
add_subdirectory(tests)

if(DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

set(CONFIG_VAR_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/config_vars.yaml)
set(CONFIG_FALLBACK_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/dynamic_config_fallback.json)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml.in ${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml)

FILE(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
