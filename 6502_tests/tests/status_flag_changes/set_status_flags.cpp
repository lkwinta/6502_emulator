//
// Created by Lukasz on 15.08.2022.
//
#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;


class M6502SetStatusFlagsTest : public testing::Test {
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

TEST_F(M6502SetStatusFlagsTest, SECCanSetC){
    //given:
    cpu.P.C = 0;

    mem[0x8000] = INSTRUCTIONS::INS_SEC; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_TRUE(cpu.P.C);

    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
    EXPECT_EQ(cpu.P.V, CPUCopy.P.V);
    EXPECT_EQ(cpu.P.Z, CPUCopy.P.Z);
    EXPECT_EQ(cpu.P.N, CPUCopy.P.N);
}

TEST_F(M6502SetStatusFlagsTest, SEDCanSetD){
    //given:
    cpu.P.D = 0;

    mem[0x8000] = INSTRUCTIONS::INS_SED; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_TRUE(cpu.P.D);

    EXPECT_EQ(cpu.P.C, CPUCopy.P.C);
    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
    EXPECT_EQ(cpu.P.V, CPUCopy.P.V);
    EXPECT_EQ(cpu.P.Z, CPUCopy.P.Z);
    EXPECT_EQ(cpu.P.N, CPUCopy.P.N);
}

TEST_F(M6502SetStatusFlagsTest, SEICanSetI){
    //given:
    cpu.P.I = 0;

    mem[0x8000] = INSTRUCTIONS::INS_SEI; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_TRUE(cpu.P.I);

    EXPECT_EQ(cpu.P.C, CPUCopy.P.C);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
    EXPECT_EQ(cpu.P.V, CPUCopy.P.V);
    EXPECT_EQ(cpu.P.Z, CPUCopy.P.Z);
    EXPECT_EQ(cpu.P.N, CPUCopy.P.N);
}
