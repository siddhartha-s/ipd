cmake_minimum_required(VERSION 3.3)
project(lec17 CXX)
include(.ipd/CMakeLists.txt)

add_test_program(bst_test
        test/bst_test.cpp
        src/Bst.h)

add_test_program(deque_test
        test/deque_test.cpp
        src/Deque.h)
