cmake_minimum_required(VERSION 3.14)

project(sequences_plugin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(rules)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(sequences_plugin SHARED
  rule_plugin_global.h
  rule_plugin.cpp
  rule_plugin.h
)

find_package(Boost COMPONENTS json REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(sequences_plugin PRIVATE ${Boost_LIBRARIES})

target_compile_definitions(sequences_plugin PRIVATE RULE_PLUGIN_LIBRARY)

include(GNUInstallDirs)
install(TARGETS sequences_plugin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(
    TARGET sequences_plugin
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sequences_plugin> ${CMAKE_BINARY_DIR}/bin
)
