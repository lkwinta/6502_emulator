//
// Created by Lukasz on 16.08.2022.
//
#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

static void VerifyUnmodifiedFlags(const CPU& cpu, const CPU& CPUCopy){
    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
}

class M6502ADCTest : public testing::Test {
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

    struct ADCTestData{
        bool Carry;
        uint8_t A;
        uint8_t operand;
        uint8_t result;

        bool ExpectedC;
        bool ExpectedZ;
        bool ExpectedN;
        bool ExpectedV;
    };


    void TestIm(ADCTestData data);
    void TestZp(ADCTestData data);
    void TestZpX(ADCTestData data);
    void TestAbs(ADCTestData data);
    void TestAbsX(ADCTestData data);
    void TestAbsY(ADCTestData data);
    void TestIndX(ADCTestData data);
    void TestIndY(ADCTestData data);
};

void M6502ADCTest::TestIm(ADCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    mem[0x8000] = INSTRUCTIONS::INS_ADC_IM;
    mem[0x8001] = data.operand;

    constexpr int32_t NUM_OF_CYCLES = 2;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, data.result);
    EXPECT_EQ(cpu.P.V, data.ExpectedV);
    EXPECT_EQ(cpu.P.C, data.ExpectedC);
    EXPECT_EQ(cpu.P.N, data.ExpectedN);
    EXPECT_EQ(cpu.P.Z, data.ExpectedZ);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502ADCTest::TestZp(ADCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    cpu.X = 0x34;
    mem[0x8000] = INSTRUCTIONS::INS_ADC_ZP;
    mem[0x8001] = 0x34;

    mem[0x0034] = data.operand;

    constexpr int32_t NUM_OF_CYCLES = 3;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, data.result);
    EXPECT_EQ(cpu.P.V, data.ExpectedV);
    EXPECT_EQ(cpu.P.C, data.ExpectedC);
    EXPECT_EQ(cpu.P.N, data.ExpectedN);
    EXPECT_EQ(cpu.P.Z, data.ExpectedZ);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502ADCTest::TestZpX(ADCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    mem[0x8000] = INSTRUCTIONS::INS_ADC_ZP_X;
    mem[0x8001] = 0x34;

    mem[0x0034] = data.operand;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, data.result);
    EXPECT_EQ(cpu.P.V, data.ExpectedV);
    EXPECT_EQ(cpu.P.C, data.ExpectedC);
    EXPECT_EQ(cpu.P.N, data.ExpectedN);
    EXPECT_EQ(cpu.P.Z, data.ExpectedZ);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502ADCTest::TestAbs(ADCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    mem[0x8000] = INSTRUCTIONS::INS_ADC_ABS;
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x40;

    mem[0x4000] = data.operand;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, data.result);
    EXPECT_EQ(cpu.P.V, data.ExpectedV);
    EXPECT_EQ(cpu.P.C, data.ExpectedC);
    EXPECT_EQ(cpu.P.N, data.ExpectedN);
    EXPECT_EQ(cpu.P.Z, data.ExpectedZ);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502ADCTest::TestAbsX(ADCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    cpu.X = 0x34;
    mem[0x8000] = INSTRUCTIONS::INS_ADC_ABS_X;
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x40;

    mem[0x4034] = data.operand;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, data.result);
    EXPECT_EQ(cpu.P.V, data.ExpectedV);
    EXPECT_EQ(cpu.P.C, data.ExpectedC);
    EXPECT_EQ(cpu.P.N, data.ExpectedN);
    EXPECT_EQ(cpu.P.Z, data.ExpectedZ);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502ADCTest::TestAbsY(ADCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    cpu.Y = 0x34;
    mem[0x8000] = INSTRUCTIONS::INS_ADC_ABS_Y;
    mem[0x8001] = 0x00;
    mem[0x8002] = 0x40;

    mem[0x4034] = data.operand;

    constexpr int32_t NUM_OF_CYCLES = 4;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, data.result);
    EXPECT_EQ(cpu.P.V, data.ExpectedV);
    EXPECT_EQ(cpu.P.C, data.ExpectedC);
    EXPECT_EQ(cpu.P.N, data.ExpectedN);
    EXPECT_EQ(cpu.P.Z, data.ExpectedZ);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502ADCTest::TestIndX(ADCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    cpu.X = 0x34;

    mem[0x8000] = INSTRUCTIONS::INS_ADC_IND_X;
    mem[0x8001] = 0x40;

    mem[0x0074] = 0x34;
    mem[0x0075] = 0x40;

    mem[0x4034] = data.operand;

    constexpr int32_t NUM_OF_CYCLES = 6;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, data.result);
    EXPECT_EQ(cpu.P.V, data.ExpectedV);
    EXPECT_EQ(cpu.P.C, data.ExpectedC);
    EXPECT_EQ(cpu.P.N, data.ExpectedN);
    EXPECT_EQ(cpu.P.Z, data.ExpectedZ);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

void M6502ADCTest::TestIndY(ADCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    cpu.Y = 0x34;

    mem[0x8000] = INSTRUCTIONS::INS_ADC_IND_Y;
    mem[0x8001] = 0x40;

    mem[0x0040] = 0x00;
    mem[0x0041] = 0x40;

    mem[0x4034] = data.operand;

    constexpr int32_t NUM_OF_CYCLES = 5;
    CPU CPUCopy = cpu;

    //when:
    int32_t totalCycles = cpu.Execute(NUM_OF_CYCLES, mem);

    //then:
    EXPECT_EQ(totalCycles, NUM_OF_CYCLES);
    EXPECT_EQ(cpu.A, data.result);
    EXPECT_EQ(cpu.P.V, data.ExpectedV);
    EXPECT_EQ(cpu.P.C, data.ExpectedC);
    EXPECT_EQ(cpu.P.N, data.ExpectedN);
    EXPECT_EQ(cpu.P.Z, data.ExpectedZ);

    VerifyUnmodifiedFlags(cpu, CPUCopy);
}

TEST_F(M6502ADCTest, ADCAbsoluteCanAddZeroAndZero){
    ADCTestData data{};
    data.A = 0x0;
    data.operand = 0x0;
    data.Carry = false;

    data.result = 0x0;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = true;

    TestAbs(data);
}

TEST_F(M6502ADCTest, ADCAbsoluteCanAddZeroAndCarry){
    ADCTestData data{};
    data.A = 0x0;
    data.operand = 0x0;
    data.Carry = true;

    data.result = 0x1;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestAbs(data);
}

TEST_F(M6502ADCTest, ADCAbsoluteCanAdd1ToFF){
    ADCTestData data{};
    data.A = 0xFF;
    data.operand = 0x1;
    data.Carry = false;

    data.result = 0x0;
    data.ExpectedC = true;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = true;

    TestAbs(data);
}

TEST_F(M6502ADCTest, ADCAbsoluteCanAddNegative){
    ADCTestData data{};
    data.A = 0x0;
    data.operand = -0x1;
    data.Carry = false;

    data.result = -0x1;
    data.ExpectedC = false;
    data.ExpectedN = true;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestAbs(data);
}

TEST_F(M6502ADCTest, ADCAbsoluteCanAddToOverflow2Negative){
    //A: 10000000
    //O: 11111111
    //R: 01111111 + C

    ADCTestData data{};
    data.A = -0x80;
    data.operand = -0x1;
    data.Carry = false;

    data.result = 0x7F;
    data.ExpectedC = true;
    data.ExpectedN = false;
    data.ExpectedV = true;
    data.ExpectedZ = false;

    TestAbs(data);
}

TEST_F(M6502ADCTest, ADCAbsoluteCanAddToOverflow2Positive){
    //A: 01111111
    //O: 00000001
    //R: 10000000

    ADCTestData data{};
    data.A = 0x7F;
    data.operand = 0x1;
    data.Carry = false;

    data.result = -0x80;
    data.ExpectedC = false;
    data.ExpectedN = true;
    data.ExpectedV = true;
    data.ExpectedZ = false;

    TestAbs(data);
}

TEST_F(M6502ADCTest, ADCAbsoluteCanAdd2PositiveNumbers){
    ADCTestData data{};
    data.A = 20;
    data.operand = 17;
    data.Carry = true;

    data.result = 38;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestAbs(data);
}

TEST_F(M6502ADCTest, ADCAbsoluteCanAddPositiveAndNegativeNumbers){
    //A: 0001 0100 20
    //O: 1110 1111 -17
    //R: 0000 0011 + C

    ADCTestData data{};
    data.A = 20;
    data.operand = -17;
    data.Carry = true;

    data.result = 4;
    data.ExpectedC = true;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestAbs(data);
}

TEST_F(M6502ADCTest, ADCAbsoluteCanAddToOverflow2NegativeAndCarry){
    //A: 10000000 -128
    //O: 11111111 -1
    //C: 00000001 1
    //R: 10000000 + C -128

    ADCTestData data{};
    data.A = -0x80;
    data.operand = -0x1;
    data.Carry = true;

    data.result = -0x80;
    data.ExpectedC = true;
    data.ExpectedN = true;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestAbs(data);
}

TEST_F(M6502ADCTest, ADCImmediateCanAddWithCarry){
    ADCTestData data{};
    data.A = 0x20;
    data.operand = 0x5;
    data.Carry = true;

    data.result = 0x26;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestIm(data);
}

TEST_F(M6502ADCTest, ADCZeroPageCanAddWithCarry){
    ADCTestData data{};
    data.A = 0x20;
    data.operand = 0x5;
    data.Carry = true;

    data.result = 0x26;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestZp(data);
}

TEST_F(M6502ADCTest, ADCZeroPageXCanAddWithCarry){
    ADCTestData data{};
    data.A = 0x20;
    data.operand = 0x5;
    data.Carry = true;

    data.result = 0x26;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestZpX(data);
}

TEST_F(M6502ADCTest, ADCAbsoluteXCanAddWithCarry){
    ADCTestData data{};
    data.A = 0x20;
    data.operand = 0x5;
    data.Carry = true;

    data.result = 0x26;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestAbsX(data);
}


TEST_F(M6502ADCTest, ADCAbsoluteYCanAddWithCarry){
    ADCTestData data{};
    data.A = 0x20;
    data.operand = 0x5;
    data.Carry = true;

    data.result = 0x26;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestAbsY(data);
}

TEST_F(M6502ADCTest, ADCIndirectXCanAddWithCarry){
    ADCTestData data{};
    data.A = 0x20;
    data.operand = 0x5;
    data.Carry = true;

    data.result = 0x26;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestIndX(data);
}

TEST_F(M6502ADCTest, ADCIndirectYCanAddWithCarry){
    ADCTestData data{};
    data.A = 0x20;
    data.operand = 0x5;
    data.Carry = true;

    data.result = 0x26;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestIndY(data);
}

TEST_F(M6502ADCTest, ADCImmediateCanAddInDecimalMode){
    cpu.P.D = true;

    ADCTestData data{};
    data.A = 0x12;
    data.operand = 0x17;
    data.Carry = false;

    data.result = 0x29;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestIm(data);
}

TEST_F(M6502ADCTest, ADCImmediateCanAddInDecimalModeCanFixBCD){
    cpu.P.D = true;

    ADCTestData data{};
    data.A = 0x09;
    data.operand = 0x09;
    data.Carry = false;

    data.result = 0x18;
    data.ExpectedC = false;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestIm(data);
}

TEST_F(M6502ADCTest, ADCImmediateCanAddInDecimalModeCanAddTo0){
    cpu.P.D = true;

    ADCTestData data{};
    data.A = 0x99;
    data.operand = 0x1;
    data.Carry = false;

    data.result = 0x00;
    data.ExpectedC = true;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = true;

    TestIm(data);
}