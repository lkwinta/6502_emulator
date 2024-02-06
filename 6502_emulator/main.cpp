#include <cstdio>
#include "6502_cpu.h"

//https://web.archive.org/web/20210604074847/http://obelisk.me.uk/6502/
using namespace MOS6502;

int main(){

    Bus mem{};
    CPU cpu{};

    /*
     *      * = 0x8000          ; tells cpu where the program is located at the memory
     *
     *      _start:             ; simple label
     *
     *              ;set value in memory which will be later loaded into A register
     *           LDY 0x15       ; {instruction at 0x8000}   load value into Y register
     *           STY #0x4312    ; {instruction at 0x8002}   store X register into 0x4312
     *
     *              ;set LSB for indirect address
     *           LDX 0x12       ; {instruction at 0x8005}   load 0x12 into X register
     *           STX 0x23       ; {instruction at 0x8007}   store X register into 0x23
     *
     *              ;set MSB for indirect address
     *           LDX 0x43       ; {instruction at 0x8009}   load 0x43 into X register
     *           STX 0x24       ; {instruction at 0x800B}   store X register int 0x24
     *
     *              ;indirect address is now stored at 2 cells: 0x23 (LSB) and 0x24 (MSB)
     *              ; load indirect address into A register
     *           LDX 0x13       ; {instruction at 0x800D}   load 0x13 into X register - offset for zero-page address
     *           LDA ($0x10, X) ; {instruction at 0x800F}   load Accumulator from indirect address X(0x13) + 0x10 -> 0x0023 and 0x0024
     *
     * Assuming that cell 0x0022 = 0x12 and 0x0023 = 0x83 with little endian conception we will get 16-bit absolute address 0x8312
     * Now assuming that cell 0x8312 = 0x15 we will load 0x15 into accumulator
     *
    * */

    const uint16_t PROGRAM_SIZE = 19;
    uint8_t program[PROGRAM_SIZE];

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


    //sets value of the reset vector ( location of the _start function)
    mem.LoadProgram( program, PROGRAM_SIZE);
    int32_t NUM_OF_CYCLES = 31;

    cpu.Reset(NUM_OF_CYCLES, mem); //7 cycles

    //We specify number of CPU cycles we want to execute
    cpu.Execute(NUM_OF_CYCLES, mem);

    printf("X: 0x%X\n", cpu.X); //0x12
    printf("A: 0x%X\n", cpu.A); //0x15

}