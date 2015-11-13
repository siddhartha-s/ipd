macro (unit_test name)
    add_test(${name}_test ${CMAKE_CURRENT_BINARY_DIR}/${name}_test)
    add_executable(${name}_test ${name}.cpp)
    target_link_libraries(${name}_test ${name} UnitTest++)
endmacro (unit_test)

unit_test(binheap)
unit_test(graph)
unit_test(union_find)
target_link_libraries(graph_test union_find)

add_test(mst_test ${CMAKE_CURRENT_BINARY_DIR}/mst_test)
add_executable(mst_test mst.cpp)
target_link_libraries(mst_test graph union_find UnitTest++)
