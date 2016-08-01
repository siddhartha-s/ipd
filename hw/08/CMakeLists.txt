cmake_minimum_required(VERSION 3.3)
project(huffman CXX)
include(.ipd/CMakeLists.txt)

set(COMMON src/bit_io.h src/bit_io.cpp src/common.h)

add_program(encode src/encode.cpp ${COMMON})
add_program(decode src/decode.cpp ${COMMON})
