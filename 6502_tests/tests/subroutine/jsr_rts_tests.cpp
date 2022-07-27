//
// Created by Lukasz on 27.07.2022.
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
    EXPECT_EQ(cpu.P.Z, CPUCopy.P.Z);
    EXPECT_EQ(cpu.P.N, CPUCopy.P.N);
}

class M6502JSRRTSTest : public testing::Test {
public:
    Memory mem{};
    CPU cpu;

    virtual void SetUp(){
        cpu.Reset(mem);
    }

    virtual void TearDown(){

    }
};

TEST_F(M6502JSRRTSTest, CanJumpToSubroutineAndJumpBack){

    cpu.Reset( mem);
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x80;

    mem[0x8000] = CPU::INSTRUCTIONS::INS_JSR; //6 cycles
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x40; //0x4000

    mem[0x4000] = CPU::INSTRUCTIONS::INS_RTS; //6 cycles

    mem[0x8003] = CPU::INSTRUCTIONS::INS_LDA_IM; //2 cycles
    mem[0x8004] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 14;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0x42);
    EXPECT_EQ(cpu.S, CPUCopy.S);
}

TEST_F(M6502JSRRTSTest, JSRAndRTSDoesNotAffectStatusFlags){

    cpu.Reset( mem);
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x80;

    mem[0x8000] = CPU::INSTRUCTIONS::INS_JSR; //6 cycles
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x40; //0x4000

    mem[0x4000] = CPU::INSTRUCTIONS::INS_RTS; //6 cycles

    constexpr int32_t NUM_OF_CYCLES = 12;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.S, CPUCopy.S);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502JSRRTSTest, CanJumpAbsolute){

    cpu.Reset( mem);
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x80;

    mem[0x8000] = CPU::INSTRUCTIONS::INS_JMP_ABS; //3 cycles
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x40; //0x4000


    mem[0x4000] = CPU::INSTRUCTIONS::INS_LDA_IM; //2 cycles
    mem[0x4001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0x42);
    EXPECT_EQ(cpu.S, CPUCopy.S);
}

TEST_F(M6502JSRRTSTest, CanJumpIndirect){

    cpu.Reset( mem);
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x80;

    mem[0x8000] = CPU::INSTRUCTIONS::INS_JMP_IND; //5 cycles
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x40; //0x4000

    mem[0x4000] = 0x00;
    mem[0x4001] = 0x20; //0x2000

    mem[0x2000] = CPU::INSTRUCTIONS::INS_LDA_IM; //2 cycles
    mem[0x2001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0x42);
}

TEST_F(M6502JSRRTSTest, JumpAbsoluteDoesNotAffectProccesorStatus){

    cpu.Reset( mem);
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x80;

    mem[0x8000] = CPU::INSTRUCTIONS::INS_JMP_ABS; //3 cycles
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x40; //0x4000

    constexpr int32_t NUM_OF_CYCLES = 3;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.S, CPUCopy.S);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502JSRRTSTest, JumpIndirectDoesNotAffectProccesorStatus){

    cpu.Reset( mem);

    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x80;

    mem[0x8000] = CPU::INSTRUCTIONS::INS_JMP_IND; //5 cycles
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x40; //0x4000

    mem[0x4000] = 0x00;
    mem[0x4001] = 0x20; //0x2000

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.S, CPUCopy.S);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}