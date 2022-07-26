//
// Created by Lukasz on 25.07.2022.
//

#ifndef INC_6502_EMULATOR_6502_CPU_H
#define INC_6502_EMULATOR_6502_CPU_H
#include <cstdint>
#include <cstdio>
#include <map>
#include <functional>

#include "Memory.h"

class CPU {
private:
    enum ADDRESSING_MODES{
        IMMEDIATE,
        ZERO_PAGE,
        ZERO_PAGE_X,
        ZERO_PAGE_Y,
        ABSOLUTE,
        ABSOLUTE_X,
        ABSOLUTE_Y,
        INDIRECT_X,
        INDIRECT_Y
    };

    uint16_t Fetch8Bits(int32_t& cycles, Memory& memory);
    uint16_t Fetch16Bits(int32_t& cycles, Memory& memory);

    static uint8_t Read8Bits(int32_t& cycles, Memory& memory, uint16_t address);
    static uint16_t Read16Bits(int32_t& cycles, Memory& memory, uint16_t address);

    void LDSetStatus(uint8_t& reg);

    void LoadRegister(ADDRESSING_MODES mode, int32_t& cycles, Memory& memory, uint8_t& reg);
    //8-bit bit field
    struct PS {
        uint8_t C : 1; //1-bit carry flag
        uint8_t Z : 1; //1-bit zero flag
        uint8_t I : 1; //1-bit interrupt disable
        uint8_t D : 1; //1-bit decimal mode
        uint8_t B : 1; //1-bit break command FLAG
        uint8_t UNUSED : 1; //1-bit unused flag
        uint8_t V : 1; //1-bit overflow flag
        uint8_t N : 1; //1-bit negative flag
    };

    void LoadLookupTable();
public:
    enum INSTRUCTIONS : uint8_t {
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZP_X = 0xB5,
        INS_LDA_ABS = 0xAD,
        INS_LDA_ABS_X = 0xBD,
        INS_LDA_ABS_Y = 0xB9,
        INS_LDA_IND_X = 0xA1,
        INS_LDA_IND_Y = 0xB1,

        INS_LDX_IM = 0xA2,
        INS_LDX_ZP = 0xA6,
        INS_LDX_ZP_Y = 0xB6,
        INS_LDX_ABS = 0xAE,
        INS_LDX_ABS_Y = 0xBE,

        INS_LDY_IM = 0xA0,
        INS_LDY_ZP = 0xA4,
        INS_LDY_ZP_X = 0xB4,
        INS_LDY_ABS = 0xAC,
        INS_LDY_ABS_X = 0xBC,

        INS_JSR = 0x20
    };

    CPU() { LoadLookupTable(); }

    std::map<INSTRUCTIONS, std::function<void(int32_t&, Memory&)>> lookupTable;

    //reset function
    void Reset(Memory& memory);
    /* return number of cycles used */
    int32_t Execute(int32_t cycles, Memory& memory);

    /////////// REGISTERS ///////////
    uint16_t PC{}; //16-bit program counter
    uint16_t S{}; //16-bit stack pointer

    uint8_t A{}; //8-bit accumulator register

    uint8_t X{}; //8-bit X register
    uint8_t Y{}; //8-bit Y register

    PS P{}; //8-bit field Processor status register
    /////////// REGISTERS ///////////
};

#endif //INC_6502_EMULATOR_6502_CPU_H
