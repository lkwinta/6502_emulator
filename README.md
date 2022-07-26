# 6502_emulator

### Projects:
 You can find 3 projects. 
  1. 6502_lib is 6502 cpu implementation. You can grab this project and include it into your project and use the cpu.
  2. 6502_test is Google Test project containing tests for each cpu instructions 
  3. 6502_emulator is simple code runner showing example how to use cpu.

### Compilation:
To compile this project you need to have CMake and MinGw installed.
  
### Usage:
In project 6502_emulator you can find ```main.cpp``` file. In this file you can program your memory
by setting fields of ```mem.Data[]``` array. Array is indexed frm ```0x000``` to ```0xFFFF```.

### Simple program:
```c++
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
```