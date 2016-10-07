cmake_minimum_required(VERSION 3.3)
project(isl++ CXX)
include(.ipd/CMakeLists.txt)

add_test_program(symbol_test
        test/symbol_test.cpp
        src/symbol.h src/symbol.cpp)

add_test_program(env_test
        test/env_test.cpp
        src/env.h
        src/symbol.h src/symbol.cpp)

add_test_program(value_test
        test/value_test.cpp
        src/struct.h
        src/value.h src/value.cpp)
