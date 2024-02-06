#include "6502_cpu.h"
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>

using namespace MOS6502;

class M6502CPUTest : public testing::Test {
public:
    Bus mem{};
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
    uint8_t program[] = {0x00, 0x80, INSTRUCTIONS::INS_LDA_IM, 0x84};
    mem.LoadProgram( program, 4);

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

    mem.LoadProgram(program, 14);
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

TEST_F(M6502CPUTest, CPUCanRunForLoopProgram){
    //given:
    int32_t c = 7;

    /*
     *
     * * = $1000
     *
     * lda #0
     * clc
     *
     * loop
     *      adc #8
     *      cmp #24
     *      bne loop
     *
     * ldx #20
     * */

    uint8_t program[] = {0x00, 0x10, 0xA9, 0x00, 0x18, 0x69,
                         0x08, 0xC9, 0x18, 0xD0, 0xFA, 0xA2,
                         0x14};

    mem.LoadProgram( program, 13);
    cpu.Reset(c, mem);

    //when:
    cpu.Execute( 40,mem);

    //then:
    EXPECT_EQ(cpu.A, 24);
    EXPECT_EQ(cpu.X, 20);
}

TEST_F(M6502CPUTest, TestEveryInstructionProgramWithoutDecimalMode){
    mem.Initialise();

    const size_t TOTAL_BYTES = 65526;

    FILE* file = fopen("bin_programs/6502_functional_test.bin", "rb");
    size_t bytes_read = fread(&mem[0x000A], 1, TOTAL_BYTES, file);
    fclose(file);

    EXPECT_EQ(bytes_read, TOTAL_BYTES);

    cpu.PC = 0x0400;

    while ( true ){
        cpu.Execute(1, mem);
        if(cpu.PC == 0x336d)
            break;
    }
    EXPECT_EQ(cpu.PC, 0x336d);
}

TEST_F(M6502CPUTest, TestEveryInstructionProgramWithDecimalMode){
    mem.Initialise();

    const size_t TOTAL_BYTES = 65526;

    FILE* file = fopen("bin_programs/6502_functional_test_decimal_mode.bin", "rb");
    size_t bytes_read = fread(&mem[0x000A], 1, TOTAL_BYTES, file);
    fclose(file);

    EXPECT_EQ(bytes_read, TOTAL_BYTES);

    cpu.PC = 0x0400;

    while ( true ){
        cpu.Execute(1, mem);
        if(cpu.PC == 0x3469)
            break;
    }
    EXPECT_EQ(cpu.PC, 0x3469);
}