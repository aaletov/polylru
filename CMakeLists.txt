cmake_minimum_required(VERSION 3.15)
project(polylru)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

if (NOT BUILD_TESTING)
  add_executable(
    ${PROJECT_NAME}
    ./main.cpp
  )
endif()

enable_testing()
include(GoogleTest)
add_executable(
  unittest
  ./lib/tests/lru/lru_test.cpp
)

target_link_libraries(unittest gtest::gtest GTest::gtest_main)
gtest_discover_tests(unittest XML_OUTPUT_DIR /home/aaletov/uni/stati/polylru/reports)
