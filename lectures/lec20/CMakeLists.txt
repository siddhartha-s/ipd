cmake_minimum_required(VERSION 3.3)
project(isl++ CXX)
include(.ipd/CMakeLists.txt)

add_test_program(value_test
    test/value_test.cpp
    src/value.h src/value.cpp)
