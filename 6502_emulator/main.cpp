#include <cstdio>
#include "6502_cpu.h"

//https://web.archive.org/web/20210604074847/http://obelisk.me.uk/6502/
using namespace MOS6502;

int main(){
    Memory mem{};
    CPU cpu{};
    cpu.Reset(mem);

    mem[0xFFFC] = CPU::INS_LDX_IM;
    mem[0xFFFD] = 0x12;

    cpu.Execute(2, mem);

    printf("0x%X\n", cpu.X);
}