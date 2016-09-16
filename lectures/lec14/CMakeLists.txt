cmake_minimum_required(VERSION 3.3)
project(lec14 CXX)
include(.ipd/CMakeLists.txt)

add_test_program(graph_test
        test/graph_test.cpp src/WU_graph.h src/WU_graph.cpp)
add_test_program(dist_heap_test
        test/dist_heap_test.cpp
        src/Distance_heap.h src/Distance_heap.cpp
        src/WU_graph.h src/WU_graph.cpp)
add_test_program(heap_test
        test/heap_test.cpp
        src/Heap.h
        src/WU_graph.h src/WU_graph.cpp)
