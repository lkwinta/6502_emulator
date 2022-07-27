#include "6502_cpu.h"
#include <gtest/gtest.h>

using namespace MOS6502;

class M6502CPUTest : public testing::Test {
public:
    Memory mem{};
    CPU cpu{};

    virtual void SetUp(){
        cpu.Reset(mem);
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
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x80;

    mem[0x8000] = CPU::INSTRUCTIONS::INS_LDA_IM;
    mem[0x8001] = 0x84;

    //when:
    CPU CPUCopy = cpu;
    int32_t cyclesUsed = cpu.Execute(1, mem);

    //then:
    EXPECT_EQ(cyclesUsed, 2);
}
