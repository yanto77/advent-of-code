cmake_minimum_required (VERSION 3.16)
project (advent_of_code)

option(ENABLE_ERRORS         "Enable extra errors"                ON)
option(ENABLE_TIME_TRACE     "Enables -ftime-trace flag"          OFF)

add_subdirectory(include/3rd-party/fmt EXCLUDE_FROM_ALL)

#
# Compilation configuration (includes, links, properties)
#

set(exec_NAME advent_of_code)
add_executable(${exec_NAME}
    src/setup/factory.cpp
    src/main.cpp

    src/helpers/print.cpp

    src/2019/intcode.cpp
    src/2019/day01.cpp
    src/2019/day02.cpp
    src/2019/day03.cpp
    src/2019/day04.cpp
    src/2019/day05.cpp
    src/2019/day06.cpp
    src/2019/day07.cpp
    src/2019/day08.cpp
    src/2019/day09.cpp
    src/2019/day10.cpp
    src/2019/day11.cpp
    src/2019/day12.cpp
    src/2019/day13.cpp
    src/2019/day14.cpp
    
    src/2020/day01.cpp
    src/2020/day02.cpp
    src/2020/day03.cpp
    src/2020/day04.cpp
    src/2020/day05.cpp
    src/2020/day06.cpp
    src/2020/day07.cpp
    src/2020/day08.cpp
    src/2020/day09.cpp
    src/2020/day10.cpp
    src/2020/day11.cpp
    src/2020/day12.cpp
    src/2020/day13.cpp
    src/2020/day14.cpp
    src/2020/day15.cpp
    src/2020/day16.cpp
    src/2020/day17.cpp
    src/2020/day18.cpp
    src/2020/day19.cpp
    src/2020/day20.cpp
    src/2020/day21.cpp
    src/2020/day22.cpp
    src/2020/day23.cpp
    src/2020/day24.cpp
    src/2020/day25.cpp

    src/2021/day01.cpp
    src/2021/day02.cpp
    src/2021/day03.cpp
    src/2021/day04.cpp
    src/2021/day05.cpp
    src/2021/day06.cpp
    src/2021/day07.cpp
    src/2021/day08.cpp
    src/2021/day09.cpp
    src/2021/day10.cpp
    src/2021/day11.cpp

    src/2022/day01.cpp
    src/2022/day02.cpp
    src/2022/day03.cpp
    src/2022/day04.cpp
)

target_precompile_headers(${exec_NAME} PRIVATE src/setup/all.h)
target_link_libraries(${exec_NAME} fmt::fmt)
target_include_directories(${exec_NAME}
    SYSTEM PRIVATE include/3rd-party/fmt/include
    SYSTEM PRIVATE include/
    SYSTEM PRIVATE src/
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif (UNIX)

set_target_properties(${exec_NAME} PROPERTIES 
    CXX_STANDARD 20

    # Used for code editor to run compilation checks on individual files.
    EXPORT_COMPILE_COMMANDS ON

    # Reduces the size of the executable/library, by hiding unnecessary info.
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

if (ENABLE_ERRORS)
    message(STATUS "Using extra compilation warnings/errors")
    target_compile_options(advent_of_code PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            # Warnings
            -Weverything
            # -Werror
            -Wall
            -Wextra
            -Wpedantic
            -pedantic
            # Disable specific warnings
            -Wno-c++98-compat-pedantic
            -Wno-exit-time-destructors
            -Wno-global-constructors
            -Wno-sign-conversion
            -Wno-sign-compare
            -Wno-padded
            -Wno-old-style-cast
            -Wno-implicit-int-float-conversion
            # Code Generation Convention
            -fwrapv
        >
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4
        >
    )

    cmake_policy(SET CMP0092 NEW) # Windows specific: Do not add /W3, as we are adding /W4 already.
endif()

if (ENABLE_TIME_TRACE)
    target_compile_options(${exec_NAME} PRIVATE -ftime-trace)
endif()

# `-rdynamic` instructs the linker to add all symbols to the dynamic symbol table 
# (as opposed to only the used ones). This allows backtrace_symbols() to convert 
# addresses to function names.
target_link_options(advent_of_code PRIVATE "-rdynamic")

# Copy static data relative to the executable
add_custom_command(TARGET ${exec_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_SOURCE_DIR}/input"
        "${CMAKE_BINARY_DIR}/input"
    COMMENT "Creating a link to input data in build dir"
)