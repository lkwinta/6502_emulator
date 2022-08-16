//
// Created by Lukasz on 16.08.2022.
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

class M6502CMPTest : public testing::Test {
public:
    Memory mem{};
    CPU cpu{};

    virtual void SetUp(){
        CPU::Setup(mem, 0x8000);
        int c = 7;
        cpu.Reset( c , mem);
    }

    virtual void TearDown(){

    }
};

TEST_F(M6502CMPTest, CMPImmediateCompareEqualValues){
    //given:

    cpu.P.Z = false;
    cpu.P.N = true;
    cpu.P.C = false;

    cpu.A = 26;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_IM;
    mem[0x8001] = 26;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);
    EXPECT_EQ(cpu.P.C, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502CMPTest, CMPImmediateCompareLargeValueToSmallValue){
    //given:

    cpu.P.Z = true;
    cpu.P.N = true;
    cpu.P.C = false;

    cpu.A = 48;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_IM;
    mem[0x8001] = 26;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);
    EXPECT_EQ(cpu.P.C, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502CMPTest, CMPImmediateCompareNegativeValueToPositiveValue){
    //given:

    cpu.P.Z = true;
    cpu.P.N = true;
    cpu.P.C = false;

    cpu.A = 130;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_IM;
    mem[0x8001] = 26;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);
    EXPECT_EQ(cpu.P.C, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502CMPTest, CMPImmediateCanSetNegativeFlag){
    //given:

    cpu.P.Z = true;
    cpu.P.N = false;
    cpu.P.C = true;

    cpu.A = 8;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_IM;
    mem[0x8001] = 26;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);
    EXPECT_EQ(cpu.P.C, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502CMPTest, CMPZeroPageCompareNegativeValueToPositiveValue){
    //given:

    cpu.P.Z = true;
    cpu.P.N = true;
    cpu.P.C = false;

    cpu.A = 130;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_ZP;
    mem[0x8001] = 0x42;

    mem[0x42] = 26;

    constexpr int32_t NUM_OF_CYCLES = 3;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);
    EXPECT_EQ(cpu.P.C, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502CMPTest, CMPZeroPageXCompareNegativeValueToPositiveValue){
    //given:

    cpu.P.Z = true;
    cpu.P.N = true;
    cpu.P.C = false;

    cpu.A = 130;
    cpu.X = 0x10;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_ZP_X;
    mem[0x8001] = 0x42;

    mem[0x52] = 26;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);
    EXPECT_EQ(cpu.P.C, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502CMPTest, CMPAbsoluteCompareNegativeValueToPositiveValue){
    //given:

    cpu.P.Z = true;
    cpu.P.N = true;
    cpu.P.C = false;

    cpu.A = 130;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_ABS;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    mem[0x4242] = 26;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);
    EXPECT_EQ(cpu.P.C, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502CMPTest, CMPAbsoluteXCompareNegativeValueToPositiveValue){
    //given:

    cpu.P.Z = true;
    cpu.P.N = true;
    cpu.P.C = false;

    cpu.A = 130;
    cpu.X = 0x10;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_ABS_X;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    mem[0x4252] = 26;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);
    EXPECT_EQ(cpu.P.C, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502CMPTest, CMPAbsoluteYCompareNegativeValueToPositiveValue){
    //given:

    cpu.P.Z = true;
    cpu.P.N = true;
    cpu.P.C = false;

    cpu.A = 130;
    cpu.Y = 0x10;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_ABS_Y;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    mem[0x4252] = 26;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);
    EXPECT_EQ(cpu.P.C, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502CMPTest, CMPIndirectXCompareNegativeValueToPositiveValue){
    //given:

    cpu.P.Z = true;
    cpu.P.N = true;
    cpu.P.C = false;

    cpu.A = 130;
    cpu.X = 0x10;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_IND_X;
    mem[0x8001] = 0x11;

    mem[0x0021] = 0x52;
    mem[0x0022] = 0x42;

    mem[0x4252] = 26;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);
    EXPECT_EQ(cpu.P.C, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502CMPTest, CMPIndirectYCompareNegativeValueToPositiveValue){
    //given:

    cpu.P.Z = true;
    cpu.P.N = true;
    cpu.P.C = false;

    cpu.A = 130;
    cpu.Y = 0x10;
    mem[0x8000] = INSTRUCTIONS::INS_CMP_IND_Y;
    mem[0x8001] = 0x21;

    mem[0x0021] = 0x42;
    mem[0x0022] = 0x42;

    mem[0x4252] = 26;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);
    EXPECT_EQ(cpu.P.C, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}