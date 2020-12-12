cmake_minimum_required (VERSION 3.10)
project (advent2020)

set(exec_NAME advent2020)
add_executable(${exec_NAME}
    src/main.cpp
    src/advent2020.cpp
    src/day01.cpp
    src/day02.cpp
    src/day03.cpp
    src/day04.cpp
    src/day05.cpp
    src/day06.cpp
    src/day07.cpp
    src/day08.cpp
    src/day09.cpp
    src/day10.cpp
    src/day11.cpp
    src/day12.cpp
    src/day13.cpp
    src/day14.cpp
    src/day15.cpp
    src/day16.cpp
    src/day17.cpp
    src/day18.cpp
    src/day19.cpp
    src/day20.cpp
    src/day21.cpp
    src/day22.cpp
    src/day23.cpp
    src/day24.cpp
    src/day25.cpp
)

target_include_directories(${exec_NAME} SYSTEM PRIVATE include/)
target_link_libraries(${exec_NAME} m)

set_target_properties(${exec_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${exec_NAME} PROPERTIES CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

target_compile_options(advent2020 PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        # Warnings
        -Weverything
        -Werror
        -Wall
        -Wextra
        -Wpedantic
        # Disable specific warnings
        -Wno-c++20-compat
        -Wno-c++98-compat-pedantic
        -Wno-unused-parameter
        -Wno-exit-time-destructors
        -Wno-global-constructors
        -Wno-sign-conversion
        -Wno-implicit-int-float-conversion
        -Wno-sign-compare
        -Wno-padded
        # Code Generation Convention
        -fwrapv
        -pedantic
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4
    >
)
