//
// Created by Lukasz on 26.07.2022.
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

class M6502STTest : public testing::Test {
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

    void STAZeroPageStore(INSTRUCTIONS instruction, uint8_t CPU::* Register);
    void STAZeroPageXYStore(INSTRUCTIONS instruction, uint8_t CPU::* Register, uint8_t CPU::* RegisterXY);
    void STAAbsoluteStore(INSTRUCTIONS instruction, uint8_t CPU::* Register);
    void STAAbsoluteXYStore(INSTRUCTIONS instruction, uint8_t CPU::* Register, uint8_t CPU::* RegisterXY);

};

void M6502STTest::STAZeroPageStore(INSTRUCTIONS instruction, uint8_t CPU::* Register) {
    //given:
    cpu.*Register = 0x2F;

    mem[0x8000] = instruction;
    mem[0x8001] = 0x80;

    mem[0x0080] = 0x31;

    constexpr int32_t NUM_OF_CYCLES = 3;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x0080], cpu.*Register);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502STTest::STAZeroPageXYStore(INSTRUCTIONS instruction, uint8_t CPU::* Register, uint8_t CPU::* RegisterXY) {
    //given:
    cpu.*Register = 0x42;
    cpu.*RegisterXY = 0x0F;

    mem[0x8000] = instruction;
    mem[0x8001] = 0x80;

    mem[0x008F] = 0x31;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x008F], cpu.*Register);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502STTest::STAAbsoluteStore(INSTRUCTIONS instruction, uint8_t CPU::*Register) {
    //given:
    cpu.*Register = 0x2F;

    mem[0x8000] = instruction;
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x40;
    mem[0x4000] = 0x31;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4000], cpu.*Register);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502STTest::STAAbsoluteXYStore(INSTRUCTIONS instruction, uint8_t CPU::*Register, uint8_t CPU::* RegisterXY) {
    //given:
    cpu.*Register = 0x2F;
    cpu.*RegisterXY = 0x92;

    mem[0x8000] = instruction;
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x20;

    mem[0x2092] = 0x31;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x2092], cpu.*Register);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502STTest, STAZeroPageCanStoreARegister) {
    STAZeroPageStore(INSTRUCTIONS::INS_STA_ZP, &CPU::A);
}

TEST_F(M6502STTest, STAZeroPageXCanStoreARegister) {
    STAZeroPageXYStore(INSTRUCTIONS::INS_STA_ZP_X, &CPU::A, &CPU::X);
}

TEST_F(M6502STTest, STAAbsoluteCanStoreARegister) {
    STAAbsoluteStore(INSTRUCTIONS::INS_STA_ABS, &CPU::A);
}

TEST_F(M6502STTest, STAAbsoluteXCanStoreARegister) {
    STAAbsoluteXYStore(INSTRUCTIONS::INS_STA_ABS_X, &CPU::A, &CPU::X);
}

TEST_F(M6502STTest, STAAbsoluteYCanStoreARegister) {
    STAAbsoluteXYStore(INSTRUCTIONS::INS_STA_ABS_Y, &CPU::A, &CPU::Y);
}

TEST_F(M6502STTest, STAIndirectXCanStoreARegister){
    cpu.A = 0x2F;
    cpu.X = 0x92;


    mem[0x8000] = INSTRUCTIONS::INS_STA_IND_X;
    mem[0x8001] = 0x20;

    mem[0x00B2] = 0x00;//0x92 + 0x20
    mem[0x00B3] = 0x40;//0x4000

    mem[0x4000] = 0x31;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4000], cpu.A);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502STTest, STAIndirectYCanStoreARegister){
    cpu.A = 0x2F;
    cpu.Y = 0x92;

    mem[0x8000] = INSTRUCTIONS::INS_STA_IND_Y;
    mem[0x8001] = 0x20;

    mem[0x0020] = 0x00;
    mem[0x0021] = 0x40; //0x4000

    mem[0x4000 + 0x92] = 0x31;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x4092], cpu.A);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}
/////////////////////////////////////////////////////////////////////////////////////

TEST_F(M6502STTest, STXZeroPageCanStoreXRegister) {
    STAZeroPageStore(INSTRUCTIONS::INS_STX_ZP, &CPU::X);
}

TEST_F(M6502STTest, STXZeroPageYCanStoreXRegister) {
    STAZeroPageXYStore(INSTRUCTIONS::INS_STX_ZP_Y, &CPU::X, &CPU::Y);
}

TEST_F(M6502STTest, STXAbsoluteCanStoreXRegister) {
    STAAbsoluteStore(INSTRUCTIONS::INS_STX_ABS, &CPU::X);
}

//////////////////////////////////////////////////////////////////////////////////////

TEST_F(M6502STTest, STYZeroPageCanStoreYRegister) {
    STAZeroPageStore(INSTRUCTIONS::INS_STY_ZP, &CPU::Y);
}

TEST_F(M6502STTest, STYZeroPageXCanStoreYRegister) {
    STAZeroPageXYStore(INSTRUCTIONS::INS_STY_ZP_X, &CPU::Y, &CPU::X);
}

TEST_F(M6502STTest, STYAbsoluteCanStoreYRegister) {
    STAAbsoluteStore(INSTRUCTIONS::INS_STY_ABS, &CPU::Y);
}


