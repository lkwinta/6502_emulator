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

TEST_F(M6502SystemFunctionsTest, RTICanReturnFromInternuptWithoutChangeInCpuState){
    //given:

    mem[0xFFFE] = 0x00;
    mem[0xFFFF] = 0x40;
    cpu.P.PS = 0x42;

    mem[0x8000] = INSTRUCTIONS::INS_BRK; //7 cycles

    mem[0x4000] = INSTRUCTIONS::INS_RTI; //6 cycles

    constexpr int32_t NUM_OF_CYCLES = 13;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.S, CPUCopy.S);
    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
    EXPECT_EQ(cpu.PC, CPUCopy.PC + 2);
}

TEST_F(M6502SystemFunctionsTest, RTICanReturnFromInternuptWithoutChangeInCpuStateWithBrkAndUnusedSet){
    //given:

    mem[0xFFFE] = 0x00;
    mem[0xFFFF] = 0x40;
    cpu.P.PS = cpu.UnusedBitFlag | cpu.BreakBitFlag;

    mem[0x8000] = INSTRUCTIONS::INS_BRK; //7 cycles

    mem[0x4000] = INSTRUCTIONS::INS_RTI; //6 cycles

    constexpr int32_t NUM_OF_CYCLES = 13;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.S, CPUCopy.S);
    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
    EXPECT_EQ(cpu.PC, CPUCopy.PC + 2);
}