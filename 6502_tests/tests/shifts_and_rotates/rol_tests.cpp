//
// Created by Lukasz on 18.08.2022.
//

#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

static void VerifyUnmodifiedFlags(const CPU& cpu, const CPU& CPUCopy){
    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
    EXPECT_EQ(cpu.P.V, CPUCopy.P.V);
}

class M6502ROLTest : public testing::Test {
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

TEST_F(M6502ROLTest, ROLACanRotateLeftTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.Z = true;
    cpu.P.N = true;

    cpu.A = 0b00100001;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_A;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b01000011);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLACanRotateLeftIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.Z = true;
    cpu.P.N = false;

    cpu.A = 0b11000001;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_A;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b10000010);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLACanRotateLeftIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.Z = false;
    cpu.P.N = true;

    cpu.A = 0b10000000;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_A;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, 0b00000000);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLZeroPageCanRotateRightTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;

    mem[0x0042] = 0b00100001;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ZP;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0042], 0b01000011);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLZeroPageCanRotateRightIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.Z = true;
    cpu.P.N = false;

    mem[0x0042] = 0b11000001;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ZP;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0042], 0b10000010);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLZeroPageCanRotateRightIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;

    mem[0x0042] = 0b10000000;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ZP;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0042], 0b00000000);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLZeroPageXCanRotateRightTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;
    cpu.X = 0x10;

    mem[0x0052] = 0b00100001;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ZP_X;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0052], 0b01000011);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLZeroPageXCanRotateRightIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.Z = true;
    cpu.P.N = false;

    cpu.X = 0x10;

    mem[0x0052] = 0b11000001;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ZP_X;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0052], 0b10000010);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLZeroPageXCanRotateRightIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;
    cpu.X = 0x10;

    mem[0x0052] = 0b10000000;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ZP_X;
    mem[0x8001] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0052], 0b00000000);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLAbsoluteCanRotateRightTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;

    mem[0x4242] = 0b00100001;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ABS;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4242], 0b01000011);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLAbsoluteCanRotateRightIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.Z = true;
    cpu.P.N = false;

    mem[0x4242] = 0b11000001;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ABS;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4242], 0b10000010);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLAbsoluteCanRotateRightIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;

    mem[0x4242] = 0b10000000;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ABS;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4242], 0b00000000);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLAbsoluteXCanRotateRightTheValue){
    //given:
    cpu.P.C = true;
    cpu.P.N = true;
    cpu.P.Z = true;
    cpu.X = 0x10;

    mem[0x4252] = 0b00100001;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ABS_X;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4252], 0b01000011);
    EXPECT_EQ(cpu.P.C, false);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLAbsoluteXCanRotateRightIntoTheCarry){
    //given:
    cpu.P.C = false;
    cpu.P.Z = true;
    cpu.P.N = false;

    cpu.X = 0x10;

    mem[0x4252] = 0b11000001;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ABS_X;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4252], 0b10000010);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, false);
    EXPECT_EQ(cpu.P.N, true);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ROLTest, ROLAbsoluteXCanRotateRightIntoTheZero){
    //given:
    cpu.P.C = false;
    cpu.P.N = true;
    cpu.P.Z = false;
    cpu.X = 0x10;

    mem[0x4252] = 0b10000000;

    mem[0x8000] = INSTRUCTIONS::INS_ROL_ABS_X;
    mem[0x8001] = 0x42;
    mem[0x8002] = 0x42;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4252], 0b00000000);
    EXPECT_EQ(cpu.P.C, true);
    EXPECT_EQ(cpu.P.Z, true);
    EXPECT_EQ(cpu.P.N, false);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}