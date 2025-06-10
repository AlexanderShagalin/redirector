cmake_minimum_required(VERSION 3.14)

project(rulegtdate_plugin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(rules)
include_directories(../../interfaces)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(rulegtdate_plugin SHARED
    rule_plugin_global.h
    rule_plugin.cpp
    rule_plugin.h
    ../../interfaces/irule.h
    rules/rulegtdate.cpp rules/rulegtdate.h
)

find_package(Boost COMPONENTS json REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(rulegtdate_plugin PRIVATE ${Boost_LIBRARIES})

target_compile_definitions(rulegtdate_plugin PRIVATE RULE_PLUGIN_LIBRARY)

add_custom_command(
    TARGET rulegtdate_plugin
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rulegtdate_plugin> ${CMAKE_BINARY_DIR}/bin
)

