cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(6502_project)

add_subdirectory(6502_lib)
add_subdirectory(6502_tests)
add_subdirectory(6502_emulator)

