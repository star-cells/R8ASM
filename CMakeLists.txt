cmake_minimum_required(VERSION 3.10)
project(R8ASM)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_library(r8asm_core STATIC ${SOURCES})

add_executable(r8asm_test tests/r8_test.cc)
target_link_libraries(r8asm_test r8asm_core)
