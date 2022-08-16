# 6502_emulator [![CMake](https://github.com/lukasz12345678/6502_emulator/actions/workflows/cmake.yml/badge.svg)](https://github.com/lukasz12345678/6502_emulator/actions/workflows/cmake.yml)

### Projects:
 You can find 3 projects. 
  1. 6502_lib is 6502 cpu implementation. You can grab this project and include it into your project and use the cpu.
  2. 6502_test is Google Test project containing tests for each cpu instructions 
  3. 6502_emulator is simple code runner showing example how to use cpu.

### Compilation:
To compile this project you need to have CMake and MinGw installed.
  
### Usage:
In project 6502_emulator you can find ```main.cpp``` file. In this file you can write program by filing byte array called ```program```.

First two bytes of the program contains memory location where program will be placed. 
Program is loaded by static method ```CPU::LoadProgram(Memory &memory, const uint8_t *program, uint8_t programSize)```

### Simple program:
```asm
; assembly 
            * = #0x8000         ; tells cpu where the program is located at the memory
            
                   ;set value in memory which will be later loaded into A register
                LDY 0x15       ; {instruction at 0x8000}   load value into Y register
                STY #0x4312    ; {instruction at 0x8002}   store X register into 0x4312
     
                   ;set LSB for indirect address
                LDX 0x12       ; {instruction at 0x8005}   load 0x12 into X register
                STX 0x23       ; {instruction at 0x8007}   store X register into 0x23
     
                   ;set MSB for indirect address
                LDX 0x43       ; {instruction at 0x8009}   load 0x43 into X register
                STX 0x24       ; {instruction at 0x800B}   store X register int 0x24
     
                   ;indirect address is now stored at 2 cells: 0x23 (LSB) and 0x24 (MSB)
                   ; load indirect address into A register
                LDX 0x13       ; {instruction at 0x800D}   load 0x13 into X register - offset for zero-page address
                LDA ($0x10, X) ; {instruction at 0x800F}   load Accumulator from indirect address X(0x13) + 0x10 -> 0x0023 and 0x0024
```

```c++
//first two bytes tells where the program will be placed
program[0] = 0x00;
program[1] = 0x80;

//set value in memory which will be later loaded into A register
program[2] = INSTRUCTIONS::INS_LDY_IM; // 2 cycles
program[3] = 0x15; //this value will be stored in ACC register by indirect address
program[4] = INSTRUCTIONS::INS_STY_ABS; //4 cycles
program[5] = 0x12;
program[6] = 0x43; //Y register will be stored at 0x4312

//set LSB for indirect address
program[7] = INSTRUCTIONS::INS_LDX_IM; // 2 cycles
program[8] = 0x12;     //set  LSB
program[9] = INSTRUCTIONS::INS_STX_ZP;  //3 cycles
program[10] = 0x23;    //store LSB in 0x22

//set MSB for indirect address
program[11] = INSTRUCTIONS::INS_LDX_IM; // 2 cycles
program[12] = 0x43;     //set  MSB
program[13] = INSTRUCTIONS::INS_STX_ZP; //3 cycles
program[14] = 0x24;    //store MSB in 0x23

//now mem[0x0023] = 0x12 (LSB) and mem[0x0024] = 0x43 (MSB)

//load indirect address into A register
program[15] = INSTRUCTIONS::INS_LDX_IM; // 2 cycles
program[16] = 0x13; //zero-page offset
program[17] = INSTRUCTIONS::INS_LDA_IND_X; // 6 cycles
program[18] = 0x10; // zero-page address for indirect address
                    // - address for A register will be read from 0x10+0x13
```
