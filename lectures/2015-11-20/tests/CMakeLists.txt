macro (unit_test name)
    add_test(${name}_test ${CMAKE_CURRENT_BINARY_DIR}/${name}_test)
    add_executable(${name}_test ${name}.cpp)
    target_link_libraries(${name}_test UnitTest++ ${ARGV})
endmacro (unit_test)

unit_test(deque)
unit_test(lru_cache)
target_link_libraries(lru_cache_test deque hash_table)
