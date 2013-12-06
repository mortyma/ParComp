cmake_minimum_required(VERSION 2.8)

project(efl2f90)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions("-Wall -Wextra -pedantic -std=c99 -D__STRICT_ANSI__ -g")
endif()

add_subdirectory(scc)


FILE(GLOB_RECURSE ALL_HEADERS *.h)
add_library(scc ${ALL_HEADERS} efl2f90.c scc/scc.c scc/scc_structures.c scc/graphstreamer.c)
