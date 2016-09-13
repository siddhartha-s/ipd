cmake_minimum_required(VERSION 3.3)
project(mst CXX)
include(.ipd/CMakeLists.txt)

add_test_program(graph_test
        test/graph_test.cpp src/WU_graph.h src/WU_graph.cpp)

add_test_program(uf_test
        test/uf_test.cpp src/Union_find.h src/Union_find.cpp)

add_test_program(mst_test
        test/mst_test.cpp
        src/mst.h src/mst.cpp
        src/WU_graph.h src/WU_graph.cpp
        src/Union_find.h src/Union_find.cpp)
