set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(codeword_test codeword.cpp)
target_link_libraries(codeword_test codeword UnitTest++)
