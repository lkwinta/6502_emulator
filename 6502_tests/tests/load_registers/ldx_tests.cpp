#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

class M6502LDXTest : public testing::Test {
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

static void VerifyUnmodifiedFlagsLDA(CPU& cpu, CPU& CPUCopy){
    EXPECT_EQ(cpu.P.C, CPUCopy.P.C);
    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
    EXPECT_EQ(cpu.P.V, CPUCopy.P.V);
}

TEST_F(M6502LDXTest, LDXImmediateCanLoadAValueIntoARegister){
    //given:

    mem[0x8000] = INSTRUCTIONS::INS_LDX_IM;
    mem[0x8001] = 0x84;
    CPU CPUCopy = cpu;
    cpu.P.N = 1;
    cpu.P.Z = 1;

    //when:
    int32_t cyclesUsed = cpu.Execute(2, mem);

    //then:
    EXPECT_EQ(cyclesUsed, 2);
    EXPECT_EQ(cpu.X, 0x84);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_TRUE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}
TEST_F(M6502LDXTest, LDXZeroPageCanLoadAValueIntoARegister){
    //given:

    mem[0x8000] = INSTRUCTIONS::INS_LDX_ZP;
    mem[0x8001] = 0x42;

    mem[0x0042] = 0x37;
    CPU CPUCopy = cpu;
    cpu.P.N = 1;
    cpu.P.Z = 1;

    //when:
    int32_t cyclesUsed = cpu.Execute(3, mem);

    //then:
    EXPECT_EQ(cyclesUsed, 3);
    EXPECT_EQ(cpu.X, 0x37);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}
TEST_F(M6502LDXTest, LDXZeroPageYCanLoadAValueIntoARegister){
    //given:
    cpu.Y = 5;

    mem[0x8000] = INSTRUCTIONS::INS_LDX_ZP_Y;
    mem[0x8001] = 0x42;

    mem[0x0047] = 0x37;
    cpu.P.N = 1;
    cpu.P.Z = 1;

    CPU CPUCopy = cpu;

    //when:
    int32_t cyclesUsed = cpu.Execute(4, mem);

    //then:
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_EQ(cpu.X, 0x37);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}
TEST_F(M6502LDXTest, LDXZeroPageYCanLoadAValueIntoARegisterWhenItWraps){
    //given:
    cpu.Y = 0xFF;

    mem[0x8000] = INSTRUCTIONS::INS_LDX_ZP_Y;
    mem[0x8001] = 0x80;
    mem[0x007F] = 0x37;
    cpu.P.N = 1;
    cpu.P.Z = 1;

    CPU CPUCopy = cpu;

    //when:
    int32_t cyclesUsed = cpu.Execute(4, mem);

    //then:
    EXPECT_EQ(cyclesUsed, 4);
    EXPECT_EQ(cpu.X, 0x37);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDXTest, LDXAbsolutCanLoadAValueIntoARegister){
    //given:
    constexpr int32_t NUM_CYCLES = 4;

    mem[0x8000] = INSTRUCTIONS::INS_LDX_ABS;
    mem[0x8001] = 0x80;
    mem[0x8002] = 0x44; //0x4480

    mem[0x4480] = 0x37;
    cpu.P.N = 1;
    cpu.P.Z = 1;

    CPU CPUCopy = cpu;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_CYCLES);
    EXPECT_EQ(cpu.X, 0x37);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDXTest, LDXAbsolutYCanLoadAValueIntoARegister){
    //given:
    constexpr int32_t NUM_CYCLES = 4;
    cpu.Y = 0x40;

    mem[0x8000] = INSTRUCTIONS::INS_LDX_ABS_Y;
    mem[0x8001] = 0x80;
    mem[0x8002] = 0x44; //0x4480

    mem[0x44C0] = 0x37; //0x4480 + 0x0040 = 0x44C0
    cpu.P.N = 1;
    cpu.P.Z = 1;

    CPU CPUCopy = cpu;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_CYCLES);
    EXPECT_EQ(cpu.X, 0x37);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}

TEST_F(M6502LDXTest, LDXAbsolutYCanLoadAValueIntoARegisterWhenItCrossesPageBoundary){
    //given:
    constexpr int32_t NUM_CYCLES = 5;
    cpu.Y = 0xFF;

    mem[0x8000] = INSTRUCTIONS::INS_LDX_ABS_Y;
    mem[0x8001] = 0x02;
    mem[0x8002] = 0x44; //0x4402

    mem[0x4501] = 0x37; //0x4402 + 0x00FF = 0x4501

    cpu.P.N = 1;
    cpu.P.Z = 1;

    CPU CPUCopy = cpu;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    //then:
    EXPECT_EQ(cyclesUsed, NUM_CYCLES);
    EXPECT_EQ(cpu.X, 0x37);
    EXPECT_FALSE(cpu.P.Z);
    EXPECT_FALSE(cpu.P.N);

    VerifyUnmodifiedFlagsLDA(cpu, CPUCopy);
}