cmake_minimum_required(VERSION 3.3)
project(isl++ CXX)
include(.ipd/CMakeLists.txt)

add_test_program(symbol_test
        test/symbol_test.cpp
        src/symbol.h src/symbol.cpp)

add_test_program(env_test
        test/env_test.cpp
        src/env.h
        src/symbol.h src/symbol.cpp)

add_test_program(value_test
        test/value_test.cpp
        src/symbol.h src/symbol.cpp
        src/env.h
        src/struct.h
        src/value.h src/value.cpp)

add_test_program(primops_test
        test/primops_test.cpp
        src/symbol.h src/symbol.cpp
        src/env.h
        src/struct.h
        src/value.h src/value.cpp
        src/primops.h src/primops.cpp)

add_test_program(eval_test
        test/eval_test.cpp
        src/eval.cpp
        src/ast.h src/ast.cpp
        src/env.h
        src/struct.h
        src/symbol.h src/symbol.cpp
        src/value.h src/value.cpp
        src/primops.h src/primops.cpp)

add_test_program(lexer_test
        src/lexer.h src/lexer.cpp
        test/lexer_test.cpp)

add_test_program(reader_test
        test/reader_test.cpp
        src/reader.h src/reader.cpp
        src/lexer.h src/lexer.cpp
        src/value.h src/value.cpp
        src/symbol.h src/symbol.cpp)

add_test_program(parser_test
        test/parser_test.cpp
        src/parser.h src/parser.cpp
        src/lexer.h src/lexer.cpp
        src/value.h src/value.cpp
        src/symbol.h src/symbol.cpp
        src/eval.cpp
        src/ast.h src/ast.cpp)
