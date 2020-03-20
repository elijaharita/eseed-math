cmake_minimum_required(VERSION 3.10)

project(eseed_math)

set(CMAKE_CXX_STANDARD 20)

# Library

add_library(eseed_math INTERFACE)
target_include_directories(eseed_math INTERFACE include/)

# Testing

enable_testing()

add_executable(eseed_math_test test/test.cpp)
target_link_libraries(eseed_math_test eseed_math)

add_test(eseed_math_test eseed_math_test)