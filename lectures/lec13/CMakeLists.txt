cmake_minimum_required(VERSION 3.3)
project(lec13 CXX)
include(.ipd/CMakeLists.txt)

add_test_program(acct_test
        test/acct_test.cpp src/Bank_account.h src/Bank_account.cpp)
add_test_program(stack_test
        test/stack_test.cpp src/Stack.h src/Stack.cpp)
add_test_program(queue_test
        test/queue_test.cpp src/Queue.h src/Queue.cpp)

add_test_program(graph_test
        test/graph_test.cpp src/WU_graph.h src/WU_graph.cpp)
add_test_program(uf_test
        test/uf_test.cpp src/Union_find.h src/Union_find.cpp)
