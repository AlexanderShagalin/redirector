cmake_minimum_required(VERSION 3.14)

project(receptor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(interfaces)
include_directories(ioc)

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

add_executable(receptor main.cpp
    interfaces/icommand.h
    interfaces/iresolvercontainer.h
    interfaces/icommandqueue.h
    interfaces/ihandler.h

    ioc/iresolvercollection.h
    ioc/iscope.h
    ioc/iscopechanger.h
    ioc/commandexception.h
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
    ioc/serverprocesscommand.h ioc/serverprocesscommand.cpp
    interfaces/iredirector.h
    httpredirector.h httpredirector.cpp
)

find_package(Boost COMPONENTS program_options json REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(receptor PRIVATE ${Boost_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS receptor
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_custom_command(
    TARGET receptor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:receptor> ${CMAKE_BINARY_DIR}/bin
)

if(ENABLE_COVERAGE)
    find_program(LCOV_PATH lcov REQUIRED)

    find_program(GENHTML_PATH genhtml REQUIRED)

    add_custom_target(coverage
        COMMAND ${LCOV_PATH} --directory . --zerocounters

        COMMAND $<TARGET_FILE:receptor>

        COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info

        COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage_filtered.info

        COMMAND ${GENHTML_PATH} coverage_filtered.info --output-directory coverage_report

        COMMAND cmake -E echo "Coverage report: file://${CMAKE_BINARY_DIR}/coverage_report/index.html"

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

        DEPENDS test_my_app
    )

endif()

if(ENABLE_TESTING)
    add_subdirectory(tests)
endif()
