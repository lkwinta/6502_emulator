//
// Created by Lukasz on 18.08.2022.
//
#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;


class M6502SystemFunctionsTest : public testing::Test {
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

TEST_F(M6502SystemFunctionsTest, BRKWillLoadAProgramCounterFromInterruptVector){
    //given:

    mem[0xFFFE] = 0x00;
    mem[0xFFFF] = 0x40;

    mem[0x8000] = INSTRUCTIONS::INS_BRK; //7 cycles

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.PC, 0x4000);
}

TEST_F(M6502SystemFunctionsTest, BRKWillLoadAProgramCounterFromInterruptVector2){
    //given:

    mem[0xFFFE] = 0x00;
    mem[0xFFFF] = 0x20;

    mem[0x8000] = INSTRUCTIONS::INS_BRK; //7 cycles

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.PC, 0x2000);
}

TEST_F(M6502SystemFunctionsTest, BRKWillPushPreviousPCToStack){
    //given:

    mem[0xFFFE] = 0x00;
    mem[0xFFFF] = 0x20;

    mem[0x8000] = INSTRUCTIONS::INS_BRK; //7 cycles

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.PC, 0x2000);
    EXPECT_EQ(mem[0x0100 + cpu.S + 2], 0x02);
    EXPECT_EQ(mem[0x0100 + cpu.S + 3], 0x80);
}

TEST_F(M6502SystemFunctionsTest, BRKWillSetBFlag){
    //given:

    mem[0xFFFE] = 0x00;
    mem[0xFFFF] = 0x20;
    cpu.P.PS = 0x0;

    mem[0x8000] = INSTRUCTIONS::INS_BRK; //7 cycles

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0100 + cpu.S + 1], cpu.UnusedBitFlag | cpu.BreakBitFlag);
}

TEST_F(M6502SystemFunctionsTest, BRKWillPushThreeBytesToStack){
    //given:

    mem[0xFFFE] = 0x00;
    mem[0xFFFF] = 0x20;

    mem[0x8000] = INSTRUCTIONS::INS_BRK; //7 cycles

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(CPUCopy.S - cpu.S, 3);
}

TEST_F(M6502SystemFunctionsTest, BRKWillPushStatusRegisterToStack){
    //given:

    mem[0xFFFE] = 0x00;
    mem[0xFFFF] = 0x20;
    cpu.P.PS = 0x42;
    mem[0x8000] = INSTRUCTIONS::INS_BRK; //7 cycles

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0100 + cpu.S + 1], 0x42 | cpu.UnusedBitFlag | cpu.BreakBitFlag);
}

TEST_F(M6502SystemFunctionsTest, BRKFullStackCheck){
    //given:

    mem[0xFFFE] = 0x00;
    mem[0xFFFF] = 0x20;

    mem[0x8000] = INSTRUCTIONS::INS_BRK; //7 cycles

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0100 + cpu.S + 1], CPUCopy.P.PS | cpu.UnusedBitFlag | cpu.BreakBitFlag);
    EXPECT_EQ(mem[0x0100 + cpu.S + 2], 0x02);
    EXPECT_EQ(mem[0x0100 + cpu.S + 3], 0x80);
    EXPECT_EQ(cpu.P.I, true);
}