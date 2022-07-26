cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(6502_emulator)

add_executable(6502_emulator main.cpp)
include_directories(${CMAKE_SOURCE_DIR}/6502_lib/headers)
target_link_libraries(6502_emulator 6502_lib)

install(TARGETS 6502_emulator RUNTIME DESTINATION bin)