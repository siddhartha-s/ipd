cmake_minimum_required(VERSION 3.3)
project(huffman CXX)
include(.ipd/CMakeLists.txt)

set(COMMON src/bit_io.h src/bit_io.cpp src/common.h)

add_program(encode src/encode.cpp src/7coder.cpp ${COMMON})
add_program(decode src/decode.cpp src/7coder.cpp ${COMMON})
add_program(huff src/huff.cpp ${COMMON})
add_program(puff src/puff.cpp ${COMMON})

add_test_program(bitio_test src/bit_io.cpp src/bit_io.h test/bitio_test.cpp)
