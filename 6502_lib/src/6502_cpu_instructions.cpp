//
// Created by Lukasz on 26.07.2022.
//

#include "6502_cpu.h"

void CPU::LoadLookupTable(){
    lookupTable = {
        /////////////////////////////////// LOAD ACCUMULATOR INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_LDA_IM,      [this](int32_t& cycles, Memory& memory) { LoadRegister(IMMEDIATE, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_ZP,      [this](int32_t& cycles, Memory& memory) { LoadRegister(ZERO_PAGE, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_ZP_X,    [this](int32_t& cycles, Memory& memory) { LoadRegister(ZERO_PAGE_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_ABS,     [this](int32_t& cycles, Memory& memory) { LoadRegister(ABSOLUTE, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_ABS_X,   [this](int32_t& cycles, Memory& memory) { LoadRegister(ABSOLUTE_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_ABS_Y,   [this](int32_t& cycles, Memory& memory) { LoadRegister(ABSOLUTE_Y, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_IND_X,   [this](int32_t& cycles, Memory& memory) { LoadRegister(INDIRECT_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_IND_Y,   [this](int32_t& cycles, Memory& memory) { LoadRegister(INDIRECT_Y, cycles, memory, A);}},
        /////////////////////////////////// LOAD ACCUMULATOR INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// LOAD X REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_LDX_IM,      [this](int32_t& cycles, Memory& memory) { LoadRegister(IMMEDIATE, cycles, memory, X);}},
        {INSTRUCTIONS::INS_LDX_ZP,      [this](int32_t& cycles, Memory& memory) { LoadRegister(ZERO_PAGE, cycles, memory, X);}},
        {INSTRUCTIONS::INS_LDX_ZP_Y,    [this](int32_t& cycles, Memory& memory) { LoadRegister(ZERO_PAGE_Y, cycles, memory, X);}},
        {INSTRUCTIONS::INS_LDX_ABS,     [this](int32_t& cycles, Memory& memory) { LoadRegister(ABSOLUTE, cycles, memory, X);}},
        {INSTRUCTIONS::INS_LDX_ABS_Y,   [this](int32_t& cycles, Memory& memory) { LoadRegister(ABSOLUTE_Y, cycles, memory, X);}},
        /////////////////////////////////// LOAD X REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// LOAD Y REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_LDY_IM,      [this](int32_t& cycles, Memory& memory) { LoadRegister(IMMEDIATE, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_LDY_ZP,      [this](int32_t& cycles, Memory& memory) { LoadRegister(ZERO_PAGE, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_LDY_ZP_X,    [this](int32_t& cycles, Memory& memory) { LoadRegister(ZERO_PAGE_X, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_LDY_ABS,     [this](int32_t& cycles, Memory& memory) { LoadRegister(ABSOLUTE, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_LDY_ABS_X,   [this](int32_t& cycles, Memory& memory) { LoadRegister(ABSOLUTE_X, cycles, memory, Y);}},
        /////////////////////////////////// LOAD Y REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        ////////////////////////////////// JUMP INSTRUCTION IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_JSR,         [this](int32_t& cycles, Memory& memory) {
                uint16_t absoluteAddress = Fetch16Bits(cycles, memory);
                memory.Write16Bits(S,PC - 1 );
                cycles -= 2;
                S += 2;
                PC = absoluteAddress;
                cycles--;
            }}
        ////////////////////////////////// JUMP INSTRUCTION IMPLEMENTATION //////////////////////////////////
    };
}
