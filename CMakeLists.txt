cmake_minimum_required(VERSION 3.9)
project(cmdparser)

enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(cmdparser.Test EXCLUDE_FROM_ALL)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
add_test(cmdparserTest cmdparser.Test/cmdparserTest)
add_dependencies(check cmdparserTest)


