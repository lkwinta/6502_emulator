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

class M6502TransferRegistersTest : public testing::Test {
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

TEST_F(M6502TransferRegistersTest, CanTransferNegativeAccumaltorToX){
    //given:
    cpu.X = 0;
    cpu.A = 0xFF;
    cpu.P.Z = 1;
    cpu.P.N = 0;
    mem[0x8000] = INSTRUCTIONS::INS_TAX; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, cpu.X);
    EXPECT_TRUE(cpu.P.N);
    EXPECT_FALSE(cpu.P.Z);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502TransferRegistersTest, CanTransferZeroAccumlatorToX){
    //given:
    cpu.X = 0xFF;
    cpu.A = 0;
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_TAX; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, cpu.X);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502TransferRegistersTest, CanTransferNegativeAccumaltorToY){
    //given:
    cpu.Y = 0;
    cpu.A = 0xFF;
    cpu.P.Z = 1;
    cpu.P.N = 0;
    mem[0x8000] = INSTRUCTIONS::INS_TAY; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, cpu.Y);
    EXPECT_TRUE(cpu.P.N);
    EXPECT_FALSE(cpu.P.Z);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502TransferRegistersTest, CanTransferZeroAccumlatorToY){
    //given:
    cpu.Y = 0xFF;
    cpu.A = 0;
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_TAY; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, cpu.Y);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502TransferRegistersTest, CanTransferNegativeXToAccumulator){
    //given:
    cpu.A = 0;
    cpu.X = 0xFF;
    cpu.P.Z = 1;
    cpu.P.N = 0;
    mem[0x8000] = INSTRUCTIONS::INS_TXA; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, cpu.X);
    EXPECT_TRUE(cpu.P.N);
    EXPECT_FALSE(cpu.P.Z);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502TransferRegistersTest, CanTransferZeroXToAccumulator){
    //given:
    cpu.A = 0xFF;
    cpu.X = 0;
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_TXA; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, cpu.X);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502TransferRegistersTest, CanTransferNegativeYToAccumulator){
    //given:
    cpu.A = 0;
    cpu.Y = 0xFF;
    cpu.P.Z = 1;
    cpu.P.N = 0;
    mem[0x8000] = INSTRUCTIONS::INS_TYA; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, cpu.Y);
    EXPECT_TRUE(cpu.P.N);
    EXPECT_FALSE(cpu.P.Z);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502TransferRegistersTest, CanTransferZeroYToAccumulator){
    //given:
    cpu.A = 0xFF;
    cpu.Y = 0;
    cpu.P.Z = 0;
    cpu.P.N = 1;
    mem[0x8000] = INSTRUCTIONS::INS_TYA; //2 cycles
    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, cpu.Y);
    EXPECT_TRUE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}