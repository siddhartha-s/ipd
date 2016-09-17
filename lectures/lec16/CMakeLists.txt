cmake_minimum_required(VERSION 3.3)
project(lec16 CXX)
include(.ipd/CMakeLists.txt)

add_test_program(hash_test
        test/hash_test.cpp
        src/Sbox_hash.h src/Sbox_hash.cpp)
