cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(6502_lib)

include_directories(headers)
add_library(6502_lib headers/6502_cpu.h headers/Bus.h src/6502_cpu_instructions.cpp src/6502_cpu.cpp headers/Instructions.h)