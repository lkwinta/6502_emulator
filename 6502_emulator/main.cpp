#include <cstdio>
#include "6502_cpu.h"

//https://web.archive.org/web/20210604074847/http://obelisk.me.uk/6502/
using namespace MOS6502;

int main(){

    Memory mem{};
    CPU cpu{};
    cpu.Reset(mem);


    /*
     *
     * LDX 0x12         ;load 0x12 into X register
     * LDA ($0x10, X)   ;load Accumulator from absolute 16-bit address read from X(0x12) + 0x10 -> 0x0022 and 0x0023
     *
     * Assuming that cell 0x0022 = 0x12 and 0x0023 = 0x83 with little endian conception we will get 16-bit absolute address 0x8312
     * Now assuming that cell 0x8312 = 0x15 we will load 0x15 into accumulator
     *
     * */

    mem[0xFFFC] = CPU::INS_LDX_IM; // 2 cycles
    mem[0xFFFD] = 0x12; // X = 20
    mem[0xFFFE] = CPU::INS_LDA_IND_X; // 6 cycles
    mem[0xFFFF] = 0x10;

    mem[0x0022] = 0x12;
    mem[0x0023] = 0x83; //little endian address 0x8312

    mem[0x8312] = 0x15;

    //We specify number of CPU cycles we want to execute
    cpu.Execute(8, mem);

    printf("X: 0x%X\n", cpu.X);
    printf("A: 0x%X\n", cpu.A);
}