cmake_minimum_required(VERSION 3.0.0)
project(svg-graph-generator VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(include)

add_library(OrderedMap src/OrderedMap.cpp include/OrderedMap.hpp)
add_library(CSVReader src/CSVReader.cpp include/CSVReader.hpp)
add_library(JsonReader src/JsonReader.cpp include/JsonReader.hpp)
add_library(Chart src/Chart.cpp include/Chart.hpp)

add_executable(svg-graph-generator src/main.cpp)
target_link_libraries(svg-graph-generator OrderedMap CSVReader JsonReader Chart)

# Copy test file to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_data.csv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_settings.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Tests
add_executable(ordered_map_test tests/ordered_map_test.cpp)
add_executable(histogram_test tests/histogram_test.cpp)
add_executable(chart_test tests/chart_test.cpp)

target_link_libraries(ordered_map_test OrderedMap)
target_link_libraries(histogram_test OrderedMap CSVReader JsonReader Chart)
target_link_libraries(chart_test OrderedMap CSVReader JsonReader Chart)


add_test(ordered_map_test_build
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target ordered_map_test
)
set_tests_properties(ordered_map_test_build PROPERTIES FIXTURES_SETUP ordered_map_test_fixture)
add_test(
    NAME ordered_map_test
    COMMAND $<TARGET_FILE:ordered_map_test>
)
set_tests_properties(ordered_map_test PROPERTIES FIXTURES_REQUIRED ordered_map_test_fixture)

add_test(histogram_test_build
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target histogram_test
)
set_tests_properties(histogram_test_build PROPERTIES FIXTURES_SETUP histogram_test_fixture)
add_test(
    NAME histogram_test
    COMMAND $<TARGET_FILE:histogram_test>
)
set_tests_properties(histogram_test PROPERTIES FIXTURES_REQUIRED histogram_test_fixture)

add_test(chart_test_build
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target chart_test
)
set_tests_properties(chart_test_build PROPERTIES FIXTURES_SETUP chart_test_fixture)
add_test(
    NAME chart_test
    COMMAND $<TARGET_FILE:chart_test>
)
set_tests_properties(chart_test PROPERTIES FIXTURES_REQUIRED chart_test_fixture)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_custom_command(TARGET svg-graph-generator POST_BUILD COMMAND ctest -C $<CONFIGURATION> --output-on-failure)