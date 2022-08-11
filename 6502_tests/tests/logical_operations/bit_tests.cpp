//
// Created by Lukasz on 10.08.2022.
//

#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

class BitTest : public testing::Test {
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

static void VerifyUnmodifiedFlagsLDA(CPU& cpu, CPU& CPUCopy){
    EXPECT_EQ(cpu.P.C, CPUCopy.P.C);
    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
}


TEST_F(BitTest, BitZeroPageZeroFlag){
    //given:
    cpu.A = 0b01010000;
    mem[0x8000] = INSTRUCTIONS::INS_BIT_ZP;
    mem[0x8001] = 0x23;
    mem[0x0023] = 0b00001001;

    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 3;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    EXPECT_FALSE(cpu.P.V);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(BitTest, BitZeroPageOverflowFlag){
    //given:
    cpu.A = 0b00010001;
    mem[0x8000] = INSTRUCTIONS::INS_BIT_ZP;
    mem[0x8001] = 0x23;
    mem[0x0023] = 0b01001001;

    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 3;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    EXPECT_TRUE(cpu.P.V);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(BitTest, BitZeroPageNegativeFlag){
    //given:
    cpu.A = 0b01010001;
    mem[0x8000] = INSTRUCTIONS::INS_BIT_ZP;
    mem[0x8001] = 0x23;
    mem[0x0023] = 0b10001001;

    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 3;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);
    EXPECT_FALSE(cpu.P.V);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(BitTest, BitAbsoluteZeroFlag){
    //given:
    cpu.A = 0b01001010;

    mem[0x8000] = INSTRUCTIONS::INS_BIT_ABS;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x24;// 0x2423
    mem[0x2423] = 0b00000101;

    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    EXPECT_FALSE(cpu.P.V);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(BitTest, BitAbsoluteOverflowFlag){
    //given:
    cpu.A = 0b00001011;

    mem[0x8000] = INSTRUCTIONS::INS_BIT_ABS;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x24;// 0x2423
    mem[0x2423] = 0b01001101;

    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    EXPECT_TRUE(cpu.P.V);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(BitTest, BitAbsoluteNegativeFlag){
    //given:
    cpu.A = 0b01001011;

    mem[0x8000] = INSTRUCTIONS::INS_BIT_ABS;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x24;// 0x2423
    mem[0x2423] = 0b10000001;

    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);
    EXPECT_FALSE(cpu.P.V);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

