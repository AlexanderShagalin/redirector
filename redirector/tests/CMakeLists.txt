cmake_minimum_required(VERSION 3.15)

project(tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DGTEST_LANGUAGE_CXX11)

set(PRJ_DIR ..)

set(PRJ_SRC_DIR ${PRJ_DIR}/)
set(PRJ_INC_DIR ${PRJ_DIR}/)

enable_testing()

find_package(Threads REQUIRED)

find_package(GTest REQUIRED)
include_directories(${INCLUDE_DIRECTORIES}, /usr/include)
include_directories(${INCLUDE_DIRECTORIES}, ${PRJ_INC_DIR})
include_directories(${INCLUDE_DIRECTORIES}, ${PRJ_DIR}/interfaces)
include_directories(${INCLUDE_DIRECTORIES}, ${PRJ_DIR}/ioc)
include_directories(${INCLUDE_DIRECTORIES}, ${PRJ_DIR}/endpoint)

include(GoogleTest)


add_executable(test_macrocommand main.cpp test_macrocommand.cpp ${PRJ_SRC_DIR}/ioc/macrocommand.cpp )
target_link_libraries(test_macrocommand PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} GTest::gmock Threads::Threads )
gtest_discover_tests(test_macrocommand)

add_executable(test_iocregistercommand main.cpp
                        test_iocregistercommand.cpp
                        ${PRJ_SRC_DIR}/ioc/scope.cpp
                        ${PRJ_SRC_DIR}/ioc/resolvercollection.cpp
                        ${PRJ_SRC_DIR}/ioc/scopecontainer.cpp
                        ${PRJ_SRC_DIR}/ioc/scopechangeradapter.cpp
                        ${PRJ_SRC_DIR}/ioc/macrocommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocsetscoperelativecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocsetscopeabsolutecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocregistercommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocnewscopecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/fallbackcommand.cpp
                        ${PRJ_SRC_DIR}/ioc/ioc.cpp
                    )
target_link_libraries(test_iocregistercommand PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} GTest::gmock Threads::Threads )
gtest_discover_tests(test_iocregistercommand)

add_executable(test_iocnewscopecommand main.cpp
                        test_iocnewscopecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/scope.cpp
                        ${PRJ_SRC_DIR}/ioc/resolvercollection.cpp
                        ${PRJ_SRC_DIR}/ioc/scopecontainer.cpp
                        ${PRJ_SRC_DIR}/ioc/scopechangeradapter.cpp
                        ${PRJ_SRC_DIR}/ioc/macrocommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocsetscoperelativecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocsetscopeabsolutecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocregistercommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocnewscopecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/fallbackcommand.cpp
                        ${PRJ_SRC_DIR}/ioc/ioc.cpp
                    )
target_link_libraries(test_iocnewscopecommand PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} GTest::gmock Threads::Threads )
gtest_discover_tests(test_iocnewscopecommand)

add_executable(test_iocsetscoperelativecommand main.cpp
                        test_iocsetscoperelativecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/scope.cpp
                        ${PRJ_SRC_DIR}/ioc/resolvercollection.cpp
                        ${PRJ_SRC_DIR}/ioc/scopecontainer.cpp
                        ${PRJ_SRC_DIR}/ioc/scopechangeradapter.cpp
                        ${PRJ_SRC_DIR}/ioc/macrocommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocsetscoperelativecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocsetscopeabsolutecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocregistercommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocnewscopecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/fallbackcommand.cpp
                        ${PRJ_SRC_DIR}/ioc/ioc.cpp
                    )
target_link_libraries(test_iocsetscoperelativecommand PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} GTest::gmock Threads::Threads )
gtest_discover_tests(test_iocsetscoperelativecommand)

add_executable(test_iocsetscopeabsolutecommand main.cpp
                        test_iocsetscopeabsolutecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/scope.cpp
                        ${PRJ_SRC_DIR}/ioc/resolvercollection.cpp
                        ${PRJ_SRC_DIR}/ioc/scopecontainer.cpp
                        ${PRJ_SRC_DIR}/ioc/scopechangeradapter.cpp
                        ${PRJ_SRC_DIR}/ioc/macrocommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocsetscoperelativecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocsetscopeabsolutecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocregistercommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocnewscopecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/fallbackcommand.cpp
                        ${PRJ_SRC_DIR}/ioc/ioc.cpp
                    )
target_link_libraries(test_iocsetscopeabsolutecommand PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} GTest::gmock Threads::Threads )
gtest_discover_tests(test_iocsetscopeabsolutecommand)

add_executable(test_ioc main.cpp 
                        test_ioc.cpp 
                        ${PRJ_SRC_DIR}/ioc/scope.cpp
                        ${PRJ_SRC_DIR}/ioc/resolvercollection.cpp
                        ${PRJ_SRC_DIR}/ioc/scopecontainer.cpp
                        ${PRJ_SRC_DIR}/ioc/scopechangeradapter.cpp
                        ${PRJ_SRC_DIR}/ioc/iocsetscoperelativecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocsetscopeabsolutecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocregistercommand.cpp
                        ${PRJ_SRC_DIR}/ioc/iocnewscopecommand.cpp
                        ${PRJ_SRC_DIR}/ioc/fallbackcommand.cpp
                        ${PRJ_SRC_DIR}/ioc/ioc.cpp
                    )
target_link_libraries(test_ioc PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} GTest::gmock Threads::Threads )
gtest_discover_tests(test_ioc)

