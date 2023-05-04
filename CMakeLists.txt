cmake_minimum_required(VERSION 3.0.0)
project(svg-graph-generator VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(include)

add_library(CSVReader src/CSVReader.cpp include/CSVReader.hpp)
add_library(JsonReader src/JsonReader.cpp include/JsonReader.hpp)
add_executable(svg-graph-generator src/main.cpp)

target_link_libraries(svg-graph-generator CSVReader JsonReader)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
