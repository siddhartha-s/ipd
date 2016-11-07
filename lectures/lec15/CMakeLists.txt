cmake_minimum_required(VERSION 3.3)
project(lecture CXX)
include(.ipd/CMakeLists.txt)

add_test_program(vec_hash_test test/Vec_hash_test.cpp src/Vec_hash.h)

add_program(collision src/Vec_hash.h src/collisions.cpp src/Various_hashes.h
        src/hamlet.cpp)

add_program(avalanche src/avalanche.cpp src/Various_hashes.h)