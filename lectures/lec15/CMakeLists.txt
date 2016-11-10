cmake_minimum_required(VERSION 3.3)
project(lecture CXX)
include(.ipd/CMakeLists.txt)

add_test_program(vec_hash_test test/Vec_hash_test.cpp src/Vec_hash.h)

add_program(collisions src/Vec_hash.h src/collisions.cpp src/Various_hashes.h
        src/hamlet.cpp)

add_program(avalanche src/avalanche.cpp src/Various_hashes.h)

add_test_program(done_vec_hash_test test-done/Vec_hash_test_done.cpp
        src-done/Vec_hash_done.h)
add_test_program(done_open_hash_test test-done/Open_hash_test_done.cpp
        src-done/Open_hash_done.h)
add_program(done_collision src-done/Vec_hash_done.h
        src-done/hash_trial_done.cpp
        src-done/collisions_done.cpp
        src-done/Various_hashes_done.h
        src/hamlet.cpp)
add_program(done_collision2 src-done/Vec_hash_done.h
        src-done/hash_trial_done.cpp
        src-done/collisions2_done.cpp
        src-done/Various_hashes_done.h
        src/hamlet.cpp)
