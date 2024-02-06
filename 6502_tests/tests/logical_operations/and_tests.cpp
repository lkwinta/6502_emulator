//
// Created by Lukasz on 26.07.2022.
//

#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

class M6502ANDTest : public testing::Test {
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

static void VerifyUnmodifiedFlagsLDA(CPU& cpu, CPU& CPUCopy){
    EXPECT_EQ(cpu.P.C, CPUCopy.P.C);
    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
    EXPECT_EQ(cpu.P.V, CPUCopy.P.V);
}

TEST_F(M6502ANDTest, ANDImmediate){
    //given:
    cpu.A = 0b01001010;
    mem[0x8000] = INSTRUCTIONS::INS_AND_IM;
    mem[0x8001] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 2;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDImmediateSetNFlag){
    //given:
    cpu.A = 0b11111111;
    mem[0x8000] = INSTRUCTIONS::INS_AND_IM;
    mem[0x8001] = 0b11111111;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 2;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b11111111 & 0b11111111);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDImmediateSetZFlag){
    //given:
    cpu.A = 0b11111111;
    mem[0x8000] = INSTRUCTIONS::INS_AND_IM;
    mem[0x8001] = 0b00000000;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 2;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b00000000 & 0b11111111);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDZeroPage){
    //given:
    cpu.A = 0b01001010;
    mem[0x8000] = INSTRUCTIONS::INS_AND_ZP;
    mem[0x8001] = 0x23;
    mem[0x0023] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 3;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDZeroPageX){
    //given:
    cpu.A = 0b01001010;
    cpu.X = 0x21;
    mem[0x8000] = INSTRUCTIONS::INS_AND_ZP_X;
    mem[0x8001] = 0x23;
    mem[0x0044] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDZeroPageXWhenItWraps){
    //given:
    cpu.A = 0b01001010;
    cpu.X = 0xFF;
    mem[0x8000] = INSTRUCTIONS::INS_AND_ZP_X;
    mem[0x8001] = 0x23;
    mem[0x0022] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDAbsolute){
    //given:
    cpu.A = 0b01001010;
    mem[0x8000] = INSTRUCTIONS::INS_AND_ABS;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x24;// 0x2423
    mem[0x2423] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDAbsoluteX){
    //given:
    cpu.A = 0b01001010;
    cpu.X = 0x24;
    mem[0x8000] = INSTRUCTIONS::INS_AND_ABS_X;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x24;// 0x2423 + 0x24
    mem[0x2447] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDAbsoluteXWhenPageCrossing){
    //given:
    cpu.A = 0b01001010;
    cpu.X = 0xFA;
    mem[0x8000] = INSTRUCTIONS::INS_AND_ABS_X;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x20;// 0x2023
    mem[0x211D] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 5;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDAbsoluteY){
    //given:
    cpu.A = 0b01001010;
    cpu.Y = 0x24;
    mem[0x8000] = INSTRUCTIONS::INS_AND_ABS_Y;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x24;// 0x2423 + 0x24
    mem[0x2447] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDAbsoluteYWhenPageCrossing){
    //given:
    cpu.A = 0b01001010;
    cpu.Y = 0xFA;
    mem[0x8000] = INSTRUCTIONS::INS_AND_ABS_Y;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x20;// 0x2023
    mem[0x211D] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 5;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDAIndirectX){
    //given:
    cpu.A = 0b01001010;
    cpu.X = 0x24;
    mem[0x8000] = INSTRUCTIONS::INS_AND_IND_X;
    mem[0x8001] = 0x23;
    mem[0x0047] = 0x47; //0x23 + 0x24 = 0x47
    mem[0x0048] = 0x24; //0x2447
    mem[0x2447] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 6;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDIndirectY){
    //given:
    cpu.A = 0b01001010;
    cpu.Y = 0x21;

    mem[0x8000] = INSTRUCTIONS::INS_AND_IND_Y;
    mem[0x8001] = 0x23;
    mem[0x0023] = 0x23;
    mem[0x0024] = 0x20;// 0x2023 + 0x21 = 0x2044

    mem[0x2044] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 5;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502ANDTest, ANDIndirectYWhenPageCrossing){
    //given:
    cpu.A = 0b01001010;
    cpu.Y = 0xFA;

    mem[0x8000] = INSTRUCTIONS::INS_AND_IND_Y;
    mem[0x8001] = 0x23;
    mem[0x0023] = 0x23;
    mem[0x0024] = 0x20;// 0x2023 + 0xFA = 0x2044

    mem[0x211D] = 0b10111010;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 6;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01001010 & 0b10111010);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}