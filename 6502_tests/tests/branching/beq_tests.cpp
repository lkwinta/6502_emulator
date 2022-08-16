//
// Created by Lukasz on 15.08.2022.
//
#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

class M6502BEQTest : public testing::Test {
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

TEST_F(M6502BEQTest, BEQUnsuccessfulBranch){
    //given:

    cpu.P.Z = 0;
    mem[0x8000] = INSTRUCTIONS::INS_BEQ; //2 cycles (branch unsuccessful)
    mem[0x8001] = 0x1;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.PC, 0x8002);

    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
}

TEST_F(M6502BEQTest, BEQCanBranchForward){
    //given:

    cpu.P.Z = 1;
    mem[0x8000] = INSTRUCTIONS::INS_BEQ; //3 cycles (branch successful, same page)
    mem[0x8001] = 0x1;

    constexpr int32_t NUM_OF_CYCLES = 3;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.PC, 0x8003);

    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
}

TEST_F(M6502BEQTest, BEQCanBranchForwardToNextPage){
    //given:

    cpu.P.Z = 1;
    mem[0x8000] = INSTRUCTIONS::INS_JMP_ABS; //3 cycles
    mem[0x8001] = 0xF0;
    mem[0x8002] = 0x80;
    mem[0x80F0] = INSTRUCTIONS::INS_BEQ; //5 cycles (branch successful, new page)
    mem[0x80F1] = 0x55;

    constexpr int32_t NUM_OF_CYCLES = 8;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.PC, 0x8147);

    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
}

TEST_F(M6502BEQTest, BEQCanBranchBackward){
    //given:

    cpu.P.Z = 1;
    mem[0x8000] = INSTRUCTIONS::INS_JMP_ABS; //3 cycles
    mem[0x8001] = 0xF0;
    mem[0x8002] = 0x80;
    mem[0x80F0] = INSTRUCTIONS::INS_BEQ; //3 cycles (branch successful, same page)
    mem[0x80F1] = static_cast<uint8_t>(-0x3);

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.PC, 0x80EF);

    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
}

TEST_F(M6502BEQTest, BEQCanBranchBackwardToNextPage){
    //given:

    cpu.P.Z = 1;
    mem[0x8000] = INSTRUCTIONS::INS_BEQ; //5 cycles (branch successful, new page)
    mem[0x8001] = -0x3;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.PC, 0x7FFF);

    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
}

TEST_F(M6502BEQTest, BEQSmallProgram){
    //given:
    /*
     * *= $8000
     * loop
     *      lda #0
     *      beq loop
     * */
    mem[0x8000] = 0xA9;
    mem[0x8001] = 0x0;
    mem[0x8002] = 0xF0;
    mem[0x8003] = 0xFC;

    constexpr int32_t NUM_OF_CYCLES = 10; //2 times loop
    CPU CPUCopy = cpu;

    CPUCopy.P.Z = 1;
    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.PC, 0x8000);

    EXPECT_EQ(cpu.P.PS, CPUCopy.P.PS);
}