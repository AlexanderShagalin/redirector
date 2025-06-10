cmake_minimum_required(VERSION 3.14)

project(ruleequal_plugin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(rules)
include_directories(../../interfaces)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(ruleequal_plugin SHARED
  rule_plugin_global.h
  rule_plugin.cpp
  rule_plugin.h
  ../../interfaces/irule.h
  rules/ruleequal.cpp rules/ruleequal.h
)

find_package(Boost COMPONENTS json REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(ruleequal_plugin PRIVATE ${Boost_LIBRARIES})

target_compile_definitions(ruleequal_plugin PRIVATE RULE_PLUGIN_LIBRARY)

add_custom_command(
    TARGET ruleequal_plugin
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ruleequal_plugin> ${CMAKE_BINARY_DIR}/bin
)
