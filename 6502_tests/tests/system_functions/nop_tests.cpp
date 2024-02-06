//
// Created by Lukasz on 15.08.2022.
//
#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;


class M6502SystemFunctionsTest : public testing::Test {
public:
    Bus mem{};
    CPU cpu;

    virtual void SetUp(){
        CPU::Setup(mem, 0x8000);
        int c = 7;
        cpu.Reset( c , mem);
    }

    virtual void TearDown(){

    }
};

TEST_F(M6502SystemFunctionsTest, NOPCanDoNothingButConsumeACycle){
    //given:

    mem[0x8000] = INSTRUCTIONS::INS_NOP; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.PC, 0x8001);

    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
    EXPECT_EQ(cpu.X, CPUCopy.X);
    EXPECT_EQ(cpu.Y, CPUCopy.Y);
    EXPECT_EQ(cpu.A, CPUCopy.A);
    EXPECT_EQ(cpu.S, CPUCopy.S);
}