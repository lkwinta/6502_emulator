//
// Created by Lukasz on 13.08.2022.
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

class M6502IncrementDecrementTest : public testing::Test {
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

TEST_F(M6502IncrementDecrementTest, INXCanIncrement){
    //given:
    cpu.X = 1;

    cpu.P.Z = 1;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_INX; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.X, 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INXCanIncrementToZero){
    //given:
    cpu.X = 0xFF;
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_INX; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.X, 0);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INXCanIncrementToNegative){
    //given:
    cpu.X = 0b01111111;
    cpu.P.Z = 1;
    cpu.P.N = 0;
    mem[0x8000] = INSTRUCTIONS::INS_INX; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.X, 128);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INYCanIncrement){
    //given:
    cpu.Y = 1;

    cpu.P.Z = 1;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_INY; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.Y, 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INYCanIncrementToZero){
    //given:
    cpu.Y = 0xFF;
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_INY; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.Y, 0);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INYCanIncrementToNegative){
    //given:
    cpu.Y = 0b01111111;
    cpu.P.Z = 1;
    cpu.P.N = 0;
    mem[0x8000] = INSTRUCTIONS::INS_INY; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.Y, 128);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DEXCanDecrement){
    //given:
    cpu.X = 3;

    cpu.P.Z = 1;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_DEX; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.X, 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DEXCanDecrementToZero){
    //given:
    cpu.X = 1;
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_DEX; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.X, 0);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DEXCanDecrementToNegative){
    //given:
    cpu.X = 0;
    cpu.P.Z = 1;
    cpu.P.N = 0;
    mem[0x8000] = INSTRUCTIONS::INS_DEX; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.X, 0xFF);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DEYCanDecrement){
    //given:
    cpu.Y = 3;

    cpu.P.Z = 1;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_DEY; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.Y, 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DEYCanDecrementToZero){
    //given:
    cpu.Y = 1;
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_DEY; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.Y, 0);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DEYCanDecrementToNegative){
    //given:
    cpu.Y = 0;
    cpu.P.Z = 1;
    cpu.P.N = 0;
    mem[0x8000] = INSTRUCTIONS::INS_DEY; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.Y, 0xFF);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INCCanIncrementToZero){
    //given:
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_INC_ZP; //2 cycles
    mem[0x8001] = 0x24;
    mem[0x0024] = 0xFF;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0024], 0);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INCCanIncrementToNegative){
    //given:
    cpu.P.Z = 1;
    cpu.P.N = 0;

    mem[0x8000] = INSTRUCTIONS::INS_INC_ZP; //2 cycles
    mem[0x8001] = 0x24;
    mem[0x0024] = 0b01111111;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0024], 0b10000000);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INCCanIncrementZeroPage){
    //given:
    cpu.P.Z = 1;
    cpu.P.N = 1;

    mem[0x8000] = INSTRUCTIONS::INS_INC_ZP; //2 cycles
    mem[0x8001] = 0x24;
    mem[0x0024] = 1;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0024], 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INCCanIncrementZeroPageX){
    //given:
    cpu.P.Z = 1;
    cpu.P.N = 1;
    cpu.X = 0x22;

    mem[0x8000] = INSTRUCTIONS::INS_INC_ZP_X; //2 cycles
    mem[0x8001] = 0x24;

    mem[0x0046] = 1;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0046], 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INCCanIncrementAbsolute){
    //given:
    cpu.P.Z = 1;
    cpu.P.N = 1;

    mem[0x8000] = INSTRUCTIONS::INS_INC_ABS; //2 cycles
    mem[0x8001] = 0x24;
    mem[0x8002] = 0x48;

    mem[0x4824] = 1;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4824], 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, INCCanIncrementAbsoluteX){
    //given:
    cpu.P.Z = 1;
    cpu.P.N = 1;
    cpu.X = 0x22;

    mem[0x8000] = INSTRUCTIONS::INS_INC_ABS_X; //2 cycles
    mem[0x8001] = 0x24;
    mem[0x8002] = 0x48;

    mem[0x4846] = 1;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4846], 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DECCanDecrementToZero){
    //given:
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_DEC_ZP; //2 cycles
    mem[0x8001] = 0x24;
    mem[0x0024] = 1;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0024], 0);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DECCanDecrementToNegative){
    //given:
    cpu.P.Z = 1;
    cpu.P.N = 0;

    mem[0x8000] = INSTRUCTIONS::INS_DEC_ZP; //2 cycles
    mem[0x8001] = 0x24;
    mem[0x0024] = 0;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0024], 0xFF);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DECCanDecrementZeroPage){
    //given:
    cpu.P.Z = 1;
    cpu.P.N = 1;

    mem[0x8000] = INSTRUCTIONS::INS_DEC_ZP; //2 cycles
    mem[0x8001] = 0x24;
    mem[0x0024] = 3;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0024], 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DECCanDecrementZeroPageX){
    //given:
    cpu.P.Z = 1;
    cpu.P.N = 1;
    cpu.X = 0x22;

    mem[0x8000] = INSTRUCTIONS::INS_DEC_ZP_X; //2 cycles
    mem[0x8001] = 0x24;

    mem[0x0046] = 3;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0046], 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DECCanDecrementAbsolute){
    //given:
    cpu.P.Z = 1;
    cpu.P.N = 1;

    mem[0x8000] = INSTRUCTIONS::INS_DEC_ABS; //2 cycles
    mem[0x8001] = 0x24;
    mem[0x8002] = 0x48;

    mem[0x4824] = 3;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4824], 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502IncrementDecrementTest, DECCanDecrementAbsoluteX){
    //given:
    cpu.P.Z = 1;
    cpu.P.N = 1;
    cpu.X = 0x22;

    mem[0x8000] = INSTRUCTIONS::INS_DEC_ABS_X; //2 cycles
    mem[0x8001] = 0x24;
    mem[0x8002] = 0x48;

    mem[0x4846] = 3;

    constexpr int32_t NUM_OF_CYCLES = 7;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4846], 2);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}