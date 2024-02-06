//
// Created by Lukasz on 18.08.2022.
//
#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

static void VerifyUnmodifiedFlags(const CPU& cpu, const CPU& CPUCopy){
    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
    EXPECT_EQ(cpu.P.V, CPUCopy.P.V);
}

class M6502ASLTest : public testing::Test {
public:
    Bus mem{};
    CPU cpu{};

    virtual void SetUp(){
        CPU::Setup(mem, 0x8000);
        int c = 7;
        cpu.Reset( c , mem);
    }

    virtual void TearDown(){

    }
};

TEST_F(M6502ASLTest, ASLACanShiftLeftTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;

    cpu.A = 0b00000001;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_A;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b00000010);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLACanShiftLeftIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.N = false;
    cpu.P.Z = true;

    cpu.A = 0b11000001;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_A;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b10000010);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLACanShiftLeftIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;

    cpu.A = 0b10000000;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_A;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b00000000);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLZeroPageCanShiftLeftTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;

    mem[0x0042] = 0b00000001;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ZP;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0042], 0b00000010);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLZeroPageCanShiftLeftIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.N = false;
    cpu.P.Z = true;

    mem[0x0042] = 0b11000001;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ZP;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0042], 0b10000010);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLZeroPageCanShiftLeftIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;

    mem[0x0042] = 0b10000000;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ZP;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0042], 0b00000000);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLZeroPageXCanShiftLeftTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;
    cpu.X = 0x10;

    mem[0x0052] = 0b00000001;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ZP_X;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0052], 0b00000010);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLZeroPageXCanShiftLeftIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.N = false;
    cpu.P.Z = true;
    cpu.X = 0x10;

    mem[0x0052] = 0b11000001;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ZP_X;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0052], 0b10000010);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLZeroPageXCanShiftLeftIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;
    cpu.X = 0x10;

    mem[0x0052] = 0b10000000;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ZP_X;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0052], 0b00000000);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLAbsoluteCanShiftLeftTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;

    mem[0x4242] = 0b00000001;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ABS;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4242], 0b00000010);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLAbsoluteCanShiftLeftIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.N = false;
    cpu.P.Z = true;

    mem[0x4242] = 0b11000001;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ABS;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4242], 0b10000010);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLAbsoluteCanShiftLeftIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;

    mem[0x4242] = 0b10000000;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ABS;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4242], 0b00000000);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLAbsoluteXCanShiftLeftTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;
    cpu.X = 0x10;

    mem[0x4252] = 0b00000001;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ABS_X;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4252], 0b00000010);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLAbsoluteXCanShiftLeftIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.N = false;
    cpu.P.Z = true;
    cpu.X = 0x10;

    mem[0x4252] = 0b11000001;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ABS_X;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4252], 0b10000010);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ASLTest, ASLAbsoluteXCanShiftLeftIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;
    cpu.X = 0x10;

    mem[0x4252] = 0b10000000;

    mem[0x8000] = INSTRUCTIONS::INS_ASL_ABS_X;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4252], 0b00000000);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}