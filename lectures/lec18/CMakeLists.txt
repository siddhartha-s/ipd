cmake_minimum_required(VERSION 3.3)
project(lec18 CXX)
include(.ipd/CMakeLists.txt)

add_test_program(heap_test
        test/heap_test.cpp
        src/Binomial_heap.h)
