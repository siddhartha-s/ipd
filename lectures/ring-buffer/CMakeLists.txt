cmake_minimum_required(VERSION 3.3)
project(ring-buffer CXX)
include(.ipd/CMakeLists.txt)

add_test_program(RP_ring-buffer-test test/RP-ring-buffer-test.cpp
    src/RP-ring-buffer.h)

add_test_program(UP_ring-buffer-test test/UP-ring-buffer-test.cpp
        src/UP-ring-buffer.h)
