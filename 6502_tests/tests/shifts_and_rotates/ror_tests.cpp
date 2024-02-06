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

class M6502RORTest : public testing::Test {
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

    const uint8_t val1 = 0b01000010;
    const uint8_t exp1 = 0b10100001;

    const uint8_t val2 = 0b01000001;
    const uint8_t exp2 = 0b00100000;

    const uint8_t val3 = 0b00000001;
    const uint8_t exp3 = 0b00000000;
};

TEST_F(M6502RORTest, RORACanRotateRightTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.Z = true;
    cpu.P.N = true;

    cpu.A = val1;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_A;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, exp1);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORACanRotateRightIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.Z = true;
    cpu.P.N = false;

    cpu.A = val2;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_A;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, exp2);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORACanRotateRightIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.Z = false;
    cpu.P.N = true;

    cpu.A = val3;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_A;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, exp3);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORZeroPageCanRotateRightTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;

    mem[0x0042] = val1;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ZP;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0042], exp1);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORZeroPageCanRotateRightIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.Z = true;
    cpu.P.N = false;

    mem[0x0042] = val2;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ZP;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0042], exp2);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORZeroPageCanRotateRightIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;

    mem[0x0042] = val3;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ZP;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0042], exp3);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORZeroPageXCanRotateRightTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;
    cpu.X = 0x10;

    mem[0x0052] = val1;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ZP_X;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0052], exp1);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORZeroPageXCanRotateRightIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.Z = true;
    cpu.P.N = false;

    cpu.X = 0x10;

    mem[0x0052] = val2;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ZP_X;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0052], exp2);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORZeroPageXCanRotateRightIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;
    cpu.X = 0x10;

    mem[0x0052] = val3;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ZP_X;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0052], exp3);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORAbsoluteCanRotateRightTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;

    mem[0x4242] = val1;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ABS;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4242], exp1);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORAbsoluteCanRotateRightIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.Z = true;
    cpu.P.N = false;

    mem[0x4242] = val2;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ABS;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4242], exp2);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORAbsoluteCanRotateRightIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;

    mem[0x4242] = val3;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ABS;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4242], exp3);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORAbsoluteXCanRotateRightTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;
    cpu.X = 0x10;

    mem[0x4252] = val1;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ABS_X;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4252], exp1);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORAbsoluteXCanRotateRightIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.Z = true;
    cpu.P.N = false;

    cpu.X = 0x10;

    mem[0x4252] = val2;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ABS_X;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4252], exp2);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502RORTest, RORAbsoluteXCanRotateRightIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;
    cpu.X = 0x10;

    mem[0x4252] = val3;

    mem[0x8000] = INSTRUCTIONS::INS_ROR_ABS_X;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4252], exp3);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}