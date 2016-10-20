cmake_minimum_required(VERSION 3.3)
project(oographics CXX)
include(.ipd/CMakeLists.txt)

add_program(animals test/animals.cpp)

add_program(red test/red.cpp)
target_link_libraries(red graphics)

set(SHAPES
        src/Drawing.h
        src/Drawing.cpp
        src/drawings.h
        src/drawings.cpp
        src/Drawing_decorator.h
        src/Drawing_decorator.cpp
        src/Gradient.h
        src/Gradient.cpp
        src/render.h
        src/render.cpp
        )

add_program(circles test/circles.cpp ${SHAPES})
target_link_libraries(circles graphics)

add_program(box test/box.cpp ${SHAPES})
target_link_libraries(box graphics)
