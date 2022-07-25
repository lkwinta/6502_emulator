cmake_minimum_required(VERSION 3.22)
project(6502_emulator)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_GetProperties(googletest)
FetchContent_MakeAvailable(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()
enable_testing()

include_directories(headers)
set(SOURCE_FILES src/main.cpp headers/6502_cpu.h src/6502_cpu.cpp)

add_executable(6502_emulator ${SOURCE_FILES})
target_link_libraries(6502_emulator gtest_main gmock_main)


