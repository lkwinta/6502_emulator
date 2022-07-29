//
// Created by Lukasz on 26.07.2022.
//

#include "6502_cpu.h"

void MOS6502::CPU::fillInstructionsLookupTable(){
    instructionsLookupTable = {
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

        /////////////////////////////////// STORE A REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_STA_ZP,      [this](int32_t& cycles, Memory& memory) { StoreRegister(ZERO_PAGE, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_ZP_X,    [this](int32_t& cycles, Memory& memory) { StoreRegister(ZERO_PAGE_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_ABS,     [this](int32_t& cycles, Memory& memory) { StoreRegister(ABSOLUTE, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_ABS_X,   [this](int32_t& cycles, Memory& memory) { StoreRegister(ABSOLUTE_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_ABS_Y,   [this](int32_t& cycles, Memory& memory) { StoreRegister(ABSOLUTE_Y, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_IND_X,   [this](int32_t& cycles, Memory& memory) { StoreRegister(INDIRECT_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_IND_Y,   [this](int32_t& cycles, Memory& memory) { StoreRegister(INDIRECT_Y, cycles, memory, A);}},
        /////////////////////////////////// STORE A REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// STORE X REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_STX_ZP,      [this](int32_t& cycles, Memory& memory) { StoreRegister(ZERO_PAGE, cycles, memory, X);}},
        {INSTRUCTIONS::INS_STX_ZP_Y,     [this](int32_t& cycles, Memory& memory) { StoreRegister(ZERO_PAGE_Y, cycles, memory, X);}},
        {INSTRUCTIONS::INS_STX_ABS,      [this](int32_t& cycles, Memory& memory) { StoreRegister(ABSOLUTE, cycles, memory, X);}},
        /////////////////////////////////// STORE X REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// STORE Y REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_STY_ZP,      [this](int32_t& cycles, Memory& memory) { StoreRegister(ZERO_PAGE, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_STY_ZP_X,    [this](int32_t& cycles, Memory& memory) { StoreRegister(ZERO_PAGE_X, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_STY_ABS,     [this](int32_t& cycles, Memory& memory) { StoreRegister(ABSOLUTE, cycles, memory, Y);}},
        /////////////////////////////////// STORE Y REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// TRANSFER REGISTERS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_TAX,      [this](int32_t& cycles, Memory& memory) { X = A; LDSetStatus(X); cycles--; }},
        {INSTRUCTIONS::INS_TXA,      [this](int32_t& cycles, Memory& memory) { A = X; LDSetStatus(A); cycles--; }},
        {INSTRUCTIONS::INS_TAY,      [this](int32_t& cycles, Memory& memory) { Y = A; LDSetStatus(Y); cycles--; }},
        {INSTRUCTIONS::INS_TYA,      [this](int32_t& cycles, Memory& memory) { A = Y; LDSetStatus(A); cycles--; }},
        {INSTRUCTIONS::INS_TSX,      [this](int32_t& cycles, Memory& memory) { X = S; LDSetStatus(X); cycles--; }},
        {INSTRUCTIONS::INS_TXS,      [this](int32_t& cycles, Memory& memory) { S = X; cycles--; }},
        /////////////////////////////////// TRANSFER REGISTERS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// STACK OPERATIONS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_PHA,      [this](int32_t& cycles, Memory& memory) { StackPush8Bits(cycles, memory, A); cycles--;}},
        {INSTRUCTIONS::INS_PHP,      [this](int32_t& cycles, Memory& memory) { StackPush8Bits(cycles, memory, P.PS); cycles--;}},
        {INSTRUCTIONS::INS_PLA,      [this](int32_t& cycles, Memory& memory) { A = StackPop8Bits(cycles, memory); cycles -= 2;
            LDSetStatus(A);}},
        {INSTRUCTIONS::INS_PLP,      [this](int32_t& cycles, Memory& memory) { P.PS = StackPop8Bits(cycles, memory); cycles -= 2;}},
        /////////////////////////////////// STACK OPERATIONS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        ////////////////////////////////// JUMP INSTRUCTION IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_JSR,         [this](int32_t& cycles, Memory& memory) {
            uint16_t absoluteAddress = Fetch16Bits(cycles, memory);
            StackPush16Bits(cycles, memory, PC - 1);
            PC = absoluteAddress;
            cycles--;
        }},
        {INSTRUCTIONS::INS_RTS,         [this](int32_t& cycles, Memory& memory) {
            PC = StackPop16Bits(cycles, memory) + 1;
            cycles -= 3;
        }},
        {INSTRUCTIONS::INS_JMP_ABS,         [this](int32_t& cycles, Memory& memory) {
            PC = getAbsoluteAddress(cycles, memory);
        }},
        {INSTRUCTIONS::INS_JMP_IND,         [this](int32_t& cycles, Memory& memory) {
            PC = Read16Bits(cycles, memory, getAbsoluteAddress(cycles, memory));
        }}
        ////////////////////////////////// JUMP INSTRUCTION IMPLEMENTATION //////////////////////////////////
    };
}
