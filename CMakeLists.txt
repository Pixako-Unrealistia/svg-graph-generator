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

add_test(
    NAME ordered_map_test
    COMMAND $<TARGET_FILE:ordered_map_test>
)
add_test(
    NAME histogram_test
    COMMAND $<TARGET_FILE:histogram_test>
)
add_test(
    NAME chart_test
    COMMAND $<TARGET_FILE:chart_test>
)
add_custom_target(ordered_map_check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS $<TARGET_FILE:ordered_map_test>)
add_custom_target(histogram_check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS $<TARGET_FILE:histogram_test>)
add_custom_target(chart_check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS $<TARGET_FILE:chart_test>)                  

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_custom_command(TARGET svg-graph-generator
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure)