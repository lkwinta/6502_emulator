//
// Created by Lukasz on 26.07.2022.
//

#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

class M6502EORTest : public testing::Test {
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
    EXPECT_EQ(cpu.P.V, CPUCopy.P.V);
}

static constexpr uint8_t bin1 = 0b11010101;
static constexpr uint8_t bin2 = 0b10011111;

TEST_F(M6502EORTest, EORImmediate){
    //given:
    cpu.A = bin1;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_IM;
    mem[0x8001] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 2;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORImmediateSetNFlag){
    //given:
    cpu.A = 0b11111111;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_IM;
    mem[0x8001] = 0b00000000;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 2;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b11111111 ^ 0b00000000);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORImmediateSetZFlag){
    //given:
    cpu.A = 0b11111111;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_IM;
    mem[0x8001] = 0b11111111;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 2;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b11111111 ^ 0b11111111);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORZeroPage){
    //given:
    cpu.A = bin1;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_ZP;
    mem[0x8001] = 0x23;
    mem[0x0023] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 3;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORZeroPageX){
    //given:
    cpu.A = bin1;
    cpu.X = 0x21;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_ZP_X;
    mem[0x8001] = 0x23;
    mem[0x0044] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORZeroPageXWhenItWraps){
    //given:
    cpu.A = bin1;
    cpu.X = 0xFF;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_ZP_X;
    mem[0x8001] = 0x23;
    mem[0x0022] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORAbsolute){
    //given:
    cpu.A = bin1;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_ABS;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x24;// 0x2423
    mem[0x2423] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORAbsoluteX){
    //given:
    cpu.A = bin1;
    cpu.X = 0x24;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_ABS_X;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x24;// 0x2423 + 0x24
    mem[0x2447] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORAbsoluteXWhenPageCrossing){
    //given:
    cpu.A = bin1;
    cpu.X = 0xFA;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_ABS_X;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x20;// 0x2023
    mem[0x211D] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 5;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORAbsoluteY){
    //given:
    cpu.A = bin1;
    cpu.Y = 0x24;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_ABS_Y;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x24;// 0x2423 + 0x24
    mem[0x2447] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 4;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORAbsoluteYWhenPageCrossing){
    //given:
    cpu.A = bin1;
    cpu.Y = 0xFA;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_ABS_Y;
    mem[0x8001] = 0x23;
    mem[0x8002] = 0x20;// 0x2023
    mem[0x211D] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 5;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORAIndirectX){
    //given:
    cpu.A = bin1;
    cpu.X = 0x24;
    mem[0x8000] = INSTRUCTIONS::INS_EOR_IND_X;
    mem[0x8001] = 0x23;
    mem[0x0047] = 0x47; //0x23 + 0x24 = 0x47
    mem[0x0048] = 0x24; //0x2447
    mem[0x2447] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 6;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORIndirectY){
    //given:
    cpu.A = bin1;
    cpu.Y = 0x21;

    mem[0x8000] = INSTRUCTIONS::INS_EOR_IND_Y;
    mem[0x8001] = 0x23;
    mem[0x0023] = 0x23;
    mem[0x0024] = 0x20;// 0x2023 + 0x21 = 0x2044

    mem[0x2044] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 5;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502EORTest, EORIndirectYWhenPageCrossing){
    //given:
    cpu.A = bin1;
    cpu.Y = 0xFA;

    mem[0x8000] = INSTRUCTIONS::INS_EOR_IND_Y;
    mem[0x8001] = 0x23;
    mem[0x0023] = 0x23;
    mem[0x0024] = 0x20;// 0x2023 + 0xFA = 0x2044

    mem[0x211D] = bin2;
    CPU CPUCopy = cpu;
    constexpr int32_t NUM_OF_CYCLES = 6;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, bin1 ^ bin2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}