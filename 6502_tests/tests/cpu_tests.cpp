#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

class M6502CPUTest : public testing::Test {
public:
    Memory mem{};
    CPU cpu{};

    virtual void SetUp(){
    }

    virtual void TearDown(){

    }
};

TEST_F(M6502CPUTest, CPUDoesNothingWhenExecutedWithZeroCycles){
    //given:
    constexpr int32_t NUM_CYCLES = 0;

    //when:
    int32_t cyclesUsed = cpu.Execute(NUM_CYCLES, mem);

    EXPECT_EQ(cyclesUsed, NUM_CYCLES);
}



TEST_F(M6502CPUTest, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByTheInstruction){
    //given:
    int32_t c = 7;
    uint8_t program[] = {0x00, 0x80, CPU::INSTRUCTIONS::INS_LDA_IM, 0x84};
    CPU::LoadProgram(mem, program, 4);

    cpu.Reset(c, mem);

    //when:
    CPU CPUCopy = cpu;
    int32_t cyclesUsed = cpu.Execute(1, mem);

    //then:
    EXPECT_EQ(cyclesUsed, 2);
    EXPECT_EQ(cpu.A, 0x84);
}

TEST_F(M6502CPUTest, CPUCanRunSimpleProgram){
    //given:
    int32_t c = 7;
    uint8_t program[] = {0x00, 0x10, 0xA9, 0xFF, 0x85, 0x90,
                         0x8D, 0x00, 0x80, 0x49, 0xCC, 0x4C,
                         0x02, 0x10};

    CPU::LoadProgram(mem, program, 14);
    cpu.Reset(c, mem);

    //when:
    for(int32_t clock = 10001; clock > 0; ){
        clock -= cpu.Execute(21, mem);
    }

    //then:
    EXPECT_EQ(mem[0x0FFF], 0x00);
    EXPECT_EQ(mem[0x1000], 0xA9);
    EXPECT_EQ(mem[0x1001], 0xFF);
    EXPECT_EQ(mem[0x1002], 0x85);
    EXPECT_EQ(mem[0x1003], 0x90);
    //...
    EXPECT_EQ(mem[0x1009], 0x4C);
    EXPECT_EQ(mem[0x100A], 0x02);
    EXPECT_EQ(mem[0x100B], 0x10);
    EXPECT_EQ(mem[0x100C], 0x00);

    EXPECT_EQ(cpu.A, 0x33);
}
