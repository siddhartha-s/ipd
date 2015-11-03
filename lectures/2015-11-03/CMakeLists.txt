cmake_minimum_required(VERSION 3.3)
project(2015-11-03 CXX)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/unittest-cpp)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(3rdparty)

#add_executable(decode src/decode.cpp src/common.hpp)
#target_link_libraries(decode bit_io)
#
#add_executable(encode src/encode.cpp src/common.hpp)
#target_link_libraries(encode bit_io)
#
#add_executable(huff src/huff.cpp src/common.hpp)
#target_link_libraries(huff bit_io huffman)
#
#add_executable(puff src/puff.cpp src/common.hpp)
#target_link_libraries(puff bit_io huffman)
