cmake_minimum_required(VERSION 3.14)

project(redirector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

include_directories(interfaces)
include_directories(ioc)
include_directories(endpoint)

option(ENABLE_TESTING "Enable testing" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_TESTING)
    enable_testing()
    if(ENABLE_COVERAGE)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            message(INFO "Coverage is enable")
            add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
            add_link_options(--coverage)
        else()
            message(WARNING "Coverage is not supported for this compiler")
        endif()
    endif()
endif()

add_executable(redirector main.cpp
    interfaces/icommand.h
    interfaces/iresolvercontainer.h
    interfaces/icommandqueue.h
    interfaces/ihandler.h
    interfaces/iendpoint.h
    interfaces/irule.h
    interfaces/iruleloader.h
    interfaces/irulebuilder.h
    interfaces/irulesequenceloader.h
    ioc/iresolvercollection.h
    ioc/iscope.h
    ioc/iscopechanger.h
    ioc/ioc.cpp ioc/ioc.h
    ioc/scope.cpp ioc/scope.h
    ioc/exceptionmanager.cpp ioc/exceptionmanager.h
    ioc/resolvercollection.cpp ioc/resolvercollection.h
    ioc/iocnewscopecommand.cpp ioc/iocnewscopecommand.h
    ioc/iocregistercommand.cpp ioc/iocregistercommand.h
    ioc/iocsetscopeabsolutecommand.cpp ioc/iocsetscopeabsolutecommand.h
    ioc/iocsetscoperelativecommand.cpp ioc/iocsetscoperelativecommand.h
    ioc/scopecontainer.cpp ioc/scopecontainer.h
    ioc/fallbackcommand.cpp ioc/fallbackcommand.h
    ioc/scopechangeradapter.cpp ioc/scopechangeradapter.h
    ioc/commandqueue.cpp ioc/commandqueue.h
    ioc/macrocommand.cpp ioc/macrocommand.h



    ioc/ruleredirect.h ioc/ruleredirect.cpp
    ioc/sendnorulecommand.h ioc/sendnorulecommand.cpp
    ioc/serverprocesscommand.h ioc/serverprocesscommand.cpp
    ioc/jsonrulebuilder.h ioc/jsonrulebuilder.cpp
    endpoint/endpoint.h endpoint/endpoint.cpp
    endpoint/jsonendpoint.h endpoint/jsonendpoint.cpp

    rulesequenceloader.h rulesequenceloader.cpp
    ruleloader.h ruleloader.cpp
)

find_package(Boost COMPONENTS program_options json REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(redirector PRIVATE ${Boost_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS redirector
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(ENABLE_COVERAGE)
    find_program(LCOV_PATH lcov REQUIRED)

    find_program(GENHTML_PATH genhtml REQUIRED)

    add_custom_target(coverage
        COMMAND ${LCOV_PATH} --directory . --zerocounters

        COMMAND $<TARGET_FILE:redirector>

        COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info

        COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage_filtered.info

        COMMAND ${GENHTML_PATH} coverage_filtered.info --output-directory coverage_report

        COMMAND cmake -E echo "Coverage report: file://${CMAKE_BINARY_DIR}/coverage_report/index.html"

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

        DEPENDS test_my_app
    )

endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_custom_command(
    TARGET redirector
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:redirector> ${CMAKE_BINARY_DIR}/bin
)

add_subdirectory(plugins)

if(ENABLE_TESTING)
    add_subdirectory(tests)
endif()
