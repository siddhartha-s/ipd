cmake_minimum_required(VERSION 3.3)
project(lec17 CXX)
include(.ipd/CMakeLists.txt)

add_test_program(deque_test
        test/deque_test.cpp
        src/deque.h)
