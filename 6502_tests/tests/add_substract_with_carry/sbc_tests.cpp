#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

static void VerifyUnmodifiedFlags(const CPU& cpu, const CPU& CPUCopy){
    EXPECT_EQ(cpu.P.I, CPUCopy.P.I);
    EXPECT_EQ(cpu.P.D, CPUCopy.P.D);
    EXPECT_EQ(cpu.P.B, CPUCopy.P.B);
}

class M6502SBCTest : public testing::Test {
public:
    Memory mem{};
    CPU cpu{};

    virtual void SetUp(){
        CPU::Setup(mem, 0x8000);
        int c = 7;
        cpu.Reset( c , mem);
    }

    virtual void TearDown(){

    }

    struct SBCTestData{
        bool Carry;
        uint8_t A;
        uint8_t operand;
        uint8_t result;

        bool ExpectedC;
        bool ExpectedZ;
        bool ExpectedN;
        bool ExpectedV;
    };


    void TestIm(SBCTestData data);
    void TestZp(SBCTestData data);
    void TestZpX(SBCTestData data);
    void TestAbs(SBCTestData data);
    void TestAbsX(SBCTestData data);
    void TestAbsY(SBCTestData data);
    void TestIndX(SBCTestData data);
    void TestIndY(SBCTestData data);
};

void M6502SBCTest::TestIm(SBCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    mem[0x8000] = INSTRUCTIONS::INS_SBC_IM;
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

void M6502SBCTest::TestZp(SBCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    cpu.X = 0x34;
    mem[0x8000] = INSTRUCTIONS::INS_SBC_ZP;
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

void M6502SBCTest::TestZpX(SBCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    mem[0x8000] = INSTRUCTIONS::INS_SBC_ZP_X;
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

void M6502SBCTest::TestAbs(SBCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    mem[0x8000] = INSTRUCTIONS::INS_SBC_ABS;
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

void M6502SBCTest::TestAbsX(SBCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    cpu.X = 0x34;
    mem[0x8000] = INSTRUCTIONS::INS_SBC_ABS_X;
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

void M6502SBCTest::TestAbsY(SBCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    cpu.Y = 0x34;
    mem[0x8000] = INSTRUCTIONS::INS_SBC_ABS_Y;
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

void M6502SBCTest::TestIndX(SBCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    cpu.X = 0x34;

    mem[0x8000] = INSTRUCTIONS::INS_SBC_IND_X;
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

void M6502SBCTest::TestIndY(SBCTestData data){
    //given:
    cpu.P.V = !data.ExpectedV;
    cpu.P.C = data.Carry;
    cpu.P.Z = !data.ExpectedZ;
    cpu.P.N = !data.ExpectedN;

    cpu.A = data.A;
    cpu.Y = 0x34;

    mem[0x8000] = INSTRUCTIONS::INS_SBC_IND_Y;
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

TEST_F(M6502SBCTest, SBCAbsoluteCanSubstractZeroAndZero){
    SBCTestData data{};
    data.A = 0x0;
    data.operand = 0x0;
    data.Carry = true;

    data.result = 0x0;
    data.ExpectedC = true;
    data.ExpectedN = false;
    data.ExpectedV = false;
    data.ExpectedZ = true;

    TestAbs(data);
}

TEST_F(M6502SBCTest, SBCAbsoluteCanSubstractZeroAndCarry){
    SBCTestData data{};
    data.A = 0x0;
    data.operand = 0x0;
    data.Carry = false;

    data.result = -0x1;
    data.ExpectedC = false;
    data.ExpectedN = true;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestAbs(data);
}

TEST_F(M6502SBCTest, SBCAbsoluteCanSubstract0ToFF){
    SBCTestData data{};
    data.A = 0x0;
    data.operand = 0x1;
    data.Carry = true;

    data.result = 0xFF;
    data.ExpectedC = false;
    data.ExpectedN = true;
    data.ExpectedV = false;
    data.ExpectedZ = false;

    TestAbs(data);
}

TEST_F(M6502SBCTest, SBCAbsoluteCanSubstractNegative){
    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCAbsoluteCanSubstractToOverflow2Negative){
    //A: 10000000
    //O: 11111111
    //R: 01111111 + C

    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCAbsoluteCanSubstractToOverflow2Positive){
    //A: 01111111
    //O: 00000001
    //R: 10000000

    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCAbsoluteCanSubstract2PositiveNumbers){
    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCAbsoluteCanSubstractPositiveAndNegativeNumbers){
    //A: 0001 0100 20
    //O: 1110 1111 -17
    //R: 0000 0011 + C

    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCAbsoluteCanSubstractToOverflow2NegativeAndCarry){
    //A: 10000000 -128
    //O: 11111111 -1
    //C: 00000001 1
    //R: 10000000 + C -128

    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCImmediateCanSubstractWithCarry){
    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCZeroPageCanSubstractWithCarry){
    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCZeroPageXCanSubstractWithCarry){
    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCAbsoluteXCanSubstractWithCarry){
    SBCTestData data{};
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


TEST_F(M6502SBCTest, SBCAbsoluteYCanSubstractWithCarry){
    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCIndirectXCanSubstractWithCarry){
    SBCTestData data{};
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

TEST_F(M6502SBCTest, SBCIndirectYCanSubstractWithCarry){
    SBCTestData data{};
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