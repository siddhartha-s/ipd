cmake_minimum_required(VERSION 3.3)
project(lec12 CXX)
include(.ipd/CMakeLists.txt)

add_program(hello src/hello.cpp)
add_program(age src/age.cpp)
add_program(math src/math.cpp)
add_program(circumference src/circumference.cpp)
add_program(f2c src/f2c.cpp src/functions.cpp)

add_test_program(functions test/functions.cpp
    src/functions.h src/functions.cpp)
add_test_program(posn test/posn.cpp
    src/posn.h src/posn.cpp)
add_test_program(vectors test/vectors.cpp
    src/vectors.h src/vectors.cpp)

