cmake_minimum_required(VERSION 3.3)
project(huffman CXX)
include(.ipd/CMakeLists.txt)

add_program(encode src/encode.cpp)
add_program(decode src/decode.cpp)
