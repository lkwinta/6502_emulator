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
        tests/transfer_registers/transfer_registers_tests.cpp
        tests/increment_decrement/inc_dec_tests.cpp
        tests/branching/beq_tests.cpp
        tests/branching/bne_tests.cpp
        tests/branching/bcc_tests.cpp
        tests/branching/bcs_tests.cpp
        tests/branching/bmi_tests.cpp
        tests/branching/bpl_tests.cpp
        tests/branching/bvc_tests.cpp
        tests/branching/bvs_tests.cpp
        tests/status_flag_changes/set_status_flags.cpp
        tests/status_flag_changes/clear_status_flags.cpp
        tests/system_functions/nop_tests.cpp
        tests/add_subtract_with_carry/adc_tests.cpp
        tests/add_subtract_with_carry/sbc_tests.cpp
        tests/add_subtract_with_carry/cmp_tests.cpp
        tests/add_subtract_with_carry/cpx_tests.cpp
        tests/add_subtract_with_carry/cpy_tests.cpp
        tests/shifts_and_rotates/asl_tests.cpp
        tests/shifts_and_rotates/lsr_tests.cpp tests/shifts_and_rotates/rol_tests.cpp tests/shifts_and_rotates/ror_tests.cpp)

target_link_libraries(6502_tests 6502_lib gtest_main gmock_main)
include_directories(${CMAKE_SOURCE_DIR}/6502_lib/headers)
