#include <stdio.h>
#include "6502_cpu.h"

int main(){
    Memory mem{};
    CPU cpu{};
    cpu.Reset(mem);

    mem[0xFFFC] = CPU::INS_LDX_IM;
    mem[0xFFFD] = 0x12;

    cpu.Execute(2, mem);

    printf("0x%X\n", cpu.X);
}