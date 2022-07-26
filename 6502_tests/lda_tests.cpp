//
// Created by Lukasz on 26.07.2022.
//

#include "6502_cpu.h"
#include <gtest/gtest.h>
//https://web.archive.org/web/20210604074847/http://obelisk.me.uk/6502/

class M6502LDATest : public testing::Test {
public:
    Mem mem{};
    CPU cpu{};

    virtual void SetUp(){
        cpu.Reset(mem);
    }

    virtual void TearDown(){

    }
};

static void VerifyUnmodifiedFlagsLDA(CPU& cpu, CPU& CPUCopy){
    EXPECT_EQ(cpu.P.C, CPUCopy.P.C);
    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
    EXPECT_EQ(cpu.P.V, CPUCopy.P.V);
}

TEST_F(M6502LDATest, LDAImmediateCanLoadAValueIntoARegister){
//given:
mem[0xFFFC] = CPU::INS_LDA_IM;
mem[0xFFFD] = 0x84;
CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(2, mem);

//then:
EXPECT_EQ(cyclesUsed, 2);
EXPECT_EQ(cpu.A, 0x84);
EXPECT_FALSE(cpu.P.Z);
EXPECT_TRUE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDASetZAndNFlags){
//given:
constexpr int32_t NUM_OF_CYCLES = 2;

mem[0xFFFC] = CPU::INS_LDA_IM;
mem[0xFFFD] = 0x22;
CPU CPUCopy = cpu;
cpu.P.N = 1;
cpu.P.Z = 1;

//when:
int32_t cyclesUsed = cpu.Execute(NUM_OF_CYCLES, mem);

//then:
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAZeroPageCanLoadAValueIntoARegister){
//given:
mem[0xFFFC] = CPU::INS_LDA_ZP;
mem[0xFFFD] = 0x42;
mem[0x0042] = 0x37;
CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(3, mem);

//then:
EXPECT_EQ(cyclesUsed, 3);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAZeroPageXCanLoadAValueIntoARegister){
//given:
cpu.X = 5;

mem[0xFFFC] = CPU::INS_LDA_ZP_X;
mem[0xFFFD] = 0x42;
mem[0x0047] = 0x37;

CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(4, mem);

//then:
EXPECT_EQ(cyclesUsed, 4);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAZeroPageXCanLoadAValueIntoARegisterWhenItWraps){
//given:
cpu.X = 0xFF;

mem[0xFFFC] = CPU::INS_LDA_ZP_X;
mem[0xFFFD] = 0x80;
mem[0x007F] = 0x37;

CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(4, mem);

//then:
EXPECT_EQ(cyclesUsed, 4);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAAbsolutCanLoadAValueIntoARegister){
//given:
constexpr int32_t NUM_CYCLES = 4;

mem[0xFFFC] = CPU::INS_LDA_ABS;
mem[0xFFFD] = 0x80;
mem[0xFFFE] = 0x44; //0x4480
mem[0x4480] = 0x37;

CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

//then:
EXPECT_EQ(cyclesUsed, NUM_CYCLES);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAAbsolutXCanLoadAValueIntoARegister){
//given:
constexpr int32_t NUM_CYCLES = 4;

cpu.X = 0x40;
mem[0xFFFC] = CPU::INS_LDA_ABS_X;
mem[0xFFFD] = 0x80;
mem[0xFFFE] = 0x44; //0x4480
mem[0x44C0] = 0x37; //0x4480 + 0x0040 = 0x44C0

CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

//then:
EXPECT_EQ(cyclesUsed, NUM_CYCLES);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAAbsolutXCanLoadAValueIntoARegisterWhenItCrossesPageBoundry){
//given:
constexpr int32_t NUM_CYCLES = 5;

cpu.X = 0xFF;
mem[0xFFFC] = CPU::INS_LDA_ABS_X;
mem[0xFFFD] = 0x02;
mem[0xFFFE] = 0x44; //0x4402
mem[0x4501] = 0x37; //0x4402 + 0x00FF = 0x4501

CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

//then:
EXPECT_EQ(cyclesUsed, NUM_CYCLES);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAAbsolutYCanLoadAValueIntoARegister){
//given:
constexpr int32_t NUM_CYCLES = 4;

cpu.Y = 0x40;
mem[0xFFFC] = CPU::INS_LDA_ABS_Y;
mem[0xFFFD] = 0x80;
mem[0xFFFE] = 0x44; //0x4480
mem[0x44C0] = 0x37; //0x4480 + 0x0040 = 0x44C0

CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

//then:
EXPECT_EQ(cyclesUsed, NUM_CYCLES);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAAbsolutYCanLoadAValueIntoARegisterWhenItCrossesPageBoundary){
//given:
constexpr int32_t NUM_CYCLES = 5;

cpu.Y = 0xFF;
mem[0xFFFC] = CPU::INS_LDA_ABS_Y;
mem[0xFFFD] = 0x02;
mem[0xFFFE] = 0x44; //0x4402
mem[0x4501] = 0x37; //0x4402 + 0x00FF = 0x4501

CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

//then:
EXPECT_EQ(cyclesUsed, NUM_CYCLES);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAIndirectXCanLoadAValueIntoARegister){
//given:
constexpr int32_t NUM_CYCLES = 6;

cpu.X = 0x04;
mem[0xFFFC] = CPU::INS_LDA_IND_X;
mem[0xFFFD] = 0x02;
mem[0x0006] = 0x00; //0x04 + 0x02 = 0x0006
mem[0x0007] = 0x80; //0x80
mem[0x8000] = 0x37;

CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

//then:
EXPECT_EQ(cyclesUsed, NUM_CYCLES);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAIndirectYCanLoadAValueIntoARegister){
//given:
constexpr int32_t NUM_CYCLES = 5;

cpu.Y = 0x04;
mem[0xFFFC] = CPU::INS_LDA_IND_Y;
mem[0xFFFD] = 0x02;
mem[0x0002] = 0x00;
mem[0x0003] = 0x80; //0x80
mem[0x8004] = 0x37; //0x8000 + 0x04 = 0x8004

CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

//then:
EXPECT_EQ(cyclesUsed, NUM_CYCLES);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDATest, LDAIndirectYCanLoadAValueIntoARegisterWhenItCrossesPageBoundary){
//given:
constexpr int32_t NUM_CYCLES = 6;

cpu.Y = 0xFF;
mem[0xFFFC] = CPU::INS_LDA_IND_Y;
mem[0xFFFD] = 0x02;
mem[0x0002] = 0x02;
mem[0x0003] = 0x80; //0x8002
mem[0x8101] = 0x37; //0x8002 + 0xFF = 0x8101

CPU CPUCopy = cpu;

//when:
int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

//then:
EXPECT_EQ(cyclesUsed, NUM_CYCLES);
EXPECT_EQ(cpu.A, 0x37);
EXPECT_FALSE(cpu.P.Z);
EXPECT_FALSE(cpu.P.N);

VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}