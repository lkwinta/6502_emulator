cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(6502_tests)

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

add_executable(6502_tests
        tests/cpu_tests.cpp
        tests/load_registers/lda_tests.cpp
        tests/load_registers/ldx_tests.cpp
        tests/load_registers/ldy_tests.cpp
        tests/store_registers/st_tests.cpp
        tests/subroutine/jsr_rts_tests.cpp
        tests/stack_operations/stack_operations.cpp/
        tests/logical_operations/and_tests.cpp
        tests/logical_operations/eor_tests.cpp
        tests/logical_operations/ora_tests.cpp
        tests/logical_operations/bit_tests.cpp
        tests/transfer_registers/transfer_registers_tests.cpp tests/increment_decrement/inc_dec_tests.cpp)

target_link_libraries(6502_tests 6502_lib gtest_main gmock_main)
include_directories(${CMAKE_SOURCE_DIR}/6502_lib/headers)
