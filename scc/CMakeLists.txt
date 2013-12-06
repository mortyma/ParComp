cmake_minimum_required(VERSION 2.8)

project(SCC)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions("-Wall -Wextra -pedantic -std=c99 -D__STRICT_ANSI__ -g")
endif()

include_directories(src)
add_executable(scc
    src/scc.c
    src/graphstreamer.c
    src/scc_structures.c
)



FILE(GLOB_RECURSE ALL_HEADERS *.h)
add_library(dummy ${ALL_HEADERS} src/scc.c)
