cmake_minimum_required(VERSION 3.3)
project(2015-11-03 CXX)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/unittest-cpp)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(3rdparty)
