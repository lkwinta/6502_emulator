//
// Created by Lukasz on 29.07.2022.
//
#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

static void VerifyUnmodifiedFlags(const CPU& cpu, const CPU& CPUCopy){
    EXPECT_EQ(cpu.P.C, CPUCopy.P.C);
    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
    EXPECT_EQ(cpu.P.V, CPUCopy.P.V);
}

class M6502StackOperationTest : public testing::Test {
public:
    Memory mem{};
    CPU cpu;

    virtual void SetUp(){
        CPU::Setup(mem, 0x8000);
        int c = 7;
        cpu.Reset( c , mem);
    }

    virtual void TearDown(){

    }
};

TEST_F(M6502StackOperationTest, CanTransferNegativeStackPointerToX){
    //given:
    cpu.X = 0;
    cpu.S = 0xFF;
    mem[0x8000] = INSTRUCTIONS::INS_TSX; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.S, cpu.X);
    EXPECT_TRUE(cpu.P.N);
    EXPECT_FALSE(cpu.P.Z);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502StackOperationTest, CanTransferZeroStackPointerToX){
    //given:
    cpu.X = 0xFF;
    cpu.S = 0;
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_TSX; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.S, cpu.X);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502StackOperationTest, CanTransferXToStackPointer){
    //given:
    cpu.X = 0x0;
    mem[0x8000] = INSTRUCTIONS::INS_TXS; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.S, cpu.X);
    EXPECT_EQ(cpu.P.Z, CPUCopy.P.Z);
    EXPECT_EQ(cpu.P.N, CPUCopy.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502StackOperationTest, CanPushAToStack){
    //given:
    cpu.A = 0x42;
    mem[0x8000] = INSTRUCTIONS::INS_PHA; //3 cycles
    constexpr int32_t NUM_OF_CYCLES = 3;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, mem[0x0100 + cpu.S + 1]);
    EXPECT_EQ(cpu.P.Z, CPUCopy.P.Z);
    EXPECT_EQ(cpu.P.N, CPUCopy.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
    EXPECT_EQ(cpu.S, 0xFE);
}

TEST_F(M6502StackOperationTest, CanPushProcessorStatusToStack){
    //given:
    cpu.A = 0x42;
    cpu.P.PS = 0x25;

    mem[0x8000] = INSTRUCTIONS::INS_PHP; //3 cycles
    constexpr int32_t NUM_OF_CYCLES = 3;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.P.PS, mem[0x0100 + cpu.S + 1]);
    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
    EXPECT_EQ(cpu.S, 0xFE);
}

TEST_F(M6502StackOperationTest, CanPopAFromStack){
    //given:
    cpu.A = 0x00;
    cpu.S = 0xFE;
    mem[0x8000] = INSTRUCTIONS::INS_PLA; //4 cycles
    mem[0x01FF] = 0x42;
    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A,0x42);
    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
    EXPECT_EQ(cpu.S, 0xFF);
}

TEST_F(M6502StackOperationTest, CanPopZeroToAFromStack){
    //given:
    cpu.A = 0x00;
    cpu.S = 0xFE;
    cpu.P.Z = 0;
    cpu.P.N = 1;

    mem[0x8000] = INSTRUCTIONS::INS_PLA; //4 cycles
    mem[0x01FF] = 0x00;
    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A,0x00);
    EXPECT_FALSE(cpu.P.N);
    EXPECT_TRUE(cpu.P.Z);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
    EXPECT_EQ(cpu.S, 0xFF);
}

TEST_F(M6502StackOperationTest, CanPopNegativeToAFromStack){
    //given:
    cpu.A = 0x00;
    cpu.S = 0xFE;
    cpu.P.N = 0;
    cpu.P.Z = 1;
    mem[0x8000] = INSTRUCTIONS::INS_PLA; //4 cycles
    mem[0x01FF] = 0xFF;
    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A,0xFF);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
    EXPECT_EQ(cpu.S, 0xFF);
}

TEST_F(M6502StackOperationTest, CanPopProcessorStatusFromStack){
    //given:
    cpu.P.PS = 0x00;
    cpu.S = 0xFE;
    mem[0x8000] = INSTRUCTIONS::INS_PLP; //4 cycles
    mem[0x01FF] = 0x42;
    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.P.PS, 0x42);
    EXPECT_EQ(cpu.S, 0xFF);
}