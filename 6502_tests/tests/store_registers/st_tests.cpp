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
    Memory mem{};
    CPU cpu{};

    virtual void SetUp(){
        cpu.Reset(mem);
    }

    virtual void TearDown(){

    }

    void STAZeroPageStore(CPU::INSTRUCTIONS instruction, uint8_t CPU::* Register);
    void STAZeroPageXYStore(CPU::INSTRUCTIONS instruction, uint8_t CPU::* Register, uint8_t CPU::* RegisterXY);
    void STAAbsoluteStore(CPU::INSTRUCTIONS instruction, uint8_t CPU::* Register);
    void STAAbsoluteXYStore(CPU::INSTRUCTIONS instruction, uint8_t CPU::* Register, uint8_t CPU::* RegisterXY);

};

void M6502STTest::STAZeroPageStore(CPU::INSTRUCTIONS instruction, uint8_t CPU::* Register) {
    //given:
    cpu.*Register = 0x2F;
    mem[0xFFFC] = instruction;
    mem[0xFFFD] = 0x80;
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

void M6502STTest::STAZeroPageXYStore(CPU::INSTRUCTIONS instruction, uint8_t CPU::* Register, uint8_t CPU::* RegisterXY) {
    //given:
    cpu.*Register = 0x42;
    cpu.*RegisterXY = 0x0F;

    mem[0xFFFC] = instruction;
    mem[0xFFFD] = 0x80;
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

void M6502STTest::STAAbsoluteStore(CPU::INSTRUCTIONS instruction, uint8_t CPU::*Register) {
    //given:
    cpu.*Register = 0x2F;
    mem[0xFFFC] = instruction;
    mem[0xFFFD] = 0x00;
    mem[0xFFFE] = 0x80;
    mem[0x8000] = 0x31;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x8000], cpu.*Register);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502STTest::STAAbsoluteXYStore(CPU::INSTRUCTIONS instruction, uint8_t CPU::*Register, uint8_t CPU::* RegisterXY) {
    //given:
    cpu.*Register = 0x2F;
    cpu.*RegisterXY = 0x92;
    mem[0xFFFC] = instruction;
    mem[0xFFFD] = 0x00;
    mem[0xFFFE] = 0x20;
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
    STAZeroPageStore(CPU::INSTRUCTIONS::INS_STA_ZP, &CPU::A);
}

TEST_F(M6502STTest, STAZeroPageXCanStoreARegister) {
    STAZeroPageXYStore(CPU::INSTRUCTIONS::INS_STA_ZP_X, &CPU::A, &CPU::X);
}

TEST_F(M6502STTest, STAAbsoluteCanStoreARegister) {
    STAAbsoluteStore(CPU::INSTRUCTIONS::INS_STA_ABS, &CPU::A);
}

TEST_F(M6502STTest, STAAbsoluteXCanStoreARegister) {
    STAAbsoluteXYStore(CPU::INSTRUCTIONS::INS_STA_ABS_X, &CPU::A, &CPU::X);
}

TEST_F(M6502STTest, STAAbsoluteYCanStoreARegister) {
    STAAbsoluteXYStore(CPU::INSTRUCTIONS::INS_STA_ABS_Y, &CPU::A, &CPU::Y);
}

TEST_F(M6502STTest, STAIndirectXCanStoreARegister){
    cpu.A = 0x2F;
    cpu.X = 0x92;
    mem[0xFFFC] = CPU::INSTRUCTIONS::INS_STA_IND_X;
    mem[0xFFFD] = 0x20;
    mem[0x00B2] = 0x00;//0x92 + 0x20
    mem[0x00B3] = 0x80;//0x8000
    mem[0x8000] = 0x31;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x8000], cpu.A);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502STTest, STAIndirectYCanStoreARegister){
    cpu.A = 0x2F;
    cpu.Y = 0x92;
    mem[0xFFFC] = CPU::INSTRUCTIONS::INS_STA_IND_Y;
    mem[0xFFFD] = 0x20;
    mem[0x0020] = 0x00;
    mem[0x0021] = 0x80; //0x8000
    mem[0x8000 + 0x92] = 0x31;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(mem[0x8092], cpu.A);
    VerifyUnmodifiedFlags(cpu, CPUCopy);
}
/////////////////////////////////////////////////////////////////////////////////////

TEST_F(M6502STTest, STXZeroPageCanStoreXRegister) {
    STAZeroPageStore(CPU::INSTRUCTIONS::INS_STX_ZP, &CPU::X);
}

TEST_F(M6502STTest, STXZeroPageYCanStoreXRegister) {
    STAZeroPageXYStore(CPU::INSTRUCTIONS::INS_STX_ZP_Y, &CPU::X, &CPU::Y);
}

TEST_F(M6502STTest, STXAbsoluteCanStoreXRegister) {
    STAAbsoluteStore(CPU::INSTRUCTIONS::INS_STX_ABS, &CPU::X);
}

//////////////////////////////////////////////////////////////////////////////////////

TEST_F(M6502STTest, STYZeroPageCanStoreYRegister) {
    STAZeroPageStore(CPU::INSTRUCTIONS::INS_STY_ZP, &CPU::Y);
}

TEST_F(M6502STTest, STYZeroPageXCanStoreYRegister) {
    STAZeroPageXYStore(CPU::INSTRUCTIONS::INS_STY_ZP_X, &CPU::Y, &CPU::X);
}

TEST_F(M6502STTest, STYAbsoluteCanStoreYRegister) {
    STAAbsoluteStore(CPU::INSTRUCTIONS::INS_STY_ABS, &CPU::Y);
}

