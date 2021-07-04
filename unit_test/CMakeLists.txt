cmake_minimum_required(VERSION 2.8.12)
project(Integer_unit_test)

# Test requires C++20
set(CXX_STANDARD 20)
add_definitions(-DDEBUG)

include(GoogleTest)

# Compile google test sources
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(integer ${PROJECT_SOURCE_DIR}/../../Integer/integer/integer.cpp)

add_executable(
  main_test
  main_test.cpp
  constructors.cpp
  observers.cpp
  equal_operator.cpp
  logical_operators.cpp
  aritmetic_operators.cpp
  comparison_operators.cpp
  input_output_operations.cpp
)
target_link_libraries(
  main_test
  integer
  ${GTEST_LIBRARIES}
  pthread
)

target_include_directories(main_test PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/../../Integer")
gtest_discover_tests(main_test)
