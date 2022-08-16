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
        {INSTRUCTIONS::INS_TAX,      [this](int32_t& cycles, Memory& memory) { X = A; SetStatusNZ(X); cycles--; }},
        {INSTRUCTIONS::INS_TXA,      [this](int32_t& cycles, Memory& memory) { A = X; SetStatusNZ(A); cycles--; }},
        {INSTRUCTIONS::INS_TAY,      [this](int32_t& cycles, Memory& memory) { Y = A; SetStatusNZ(Y); cycles--; }},
        {INSTRUCTIONS::INS_TYA,      [this](int32_t& cycles, Memory& memory) { A = Y; SetStatusNZ(A); cycles--; }},
        {INSTRUCTIONS::INS_TSX,      [this](int32_t& cycles, Memory& memory) { X = S; SetStatusNZ(X); cycles--; }},
        {INSTRUCTIONS::INS_TXS,      [this](int32_t& cycles, Memory& memory) { S = X; cycles--; }},
        /////////////////////////////////// TRANSFER REGISTERS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// STACK OPERATIONS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_PHA,      [this](int32_t& cycles, Memory& memory) { StackPush8Bits(cycles, memory, A); cycles--;}},
        {INSTRUCTIONS::INS_PHP,      [this](int32_t& cycles, Memory& memory) { StackPush8Bits(cycles, memory, P.PS); cycles--;}},
        {INSTRUCTIONS::INS_PLA,      [this](int32_t& cycles, Memory& memory) { A = StackPop8Bits(cycles, memory); cycles -= 2;
            SetStatusNZ(A);}},
        {INSTRUCTIONS::INS_PLP,      [this](int32_t& cycles, Memory& memory) { P.PS = StackPop8Bits(cycles, memory); cycles -= 2;}},
        /////////////////////////////////// STACK OPERATIONS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// LOGICAL OPERATIONS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        //AND
        {INSTRUCTIONS::INS_AND_IM,      [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(IMMEDIATE, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_ZP,      [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ZERO_PAGE, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_ZP_X,    [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ZERO_PAGE_X, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_ABS,     [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ABSOLUTE, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_ABS_X,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ABSOLUTE_X, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_ABS_Y,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ABSOLUTE_Y, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_IND_X,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(INDIRECT_X, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_IND_Y,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(INDIRECT_Y, LOGICAL_OPERATION::AND, cycles, memory);}},
        //EOR
        {INSTRUCTIONS::INS_ORA_IM,      [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(IMMEDIATE, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_ZP,      [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ZERO_PAGE, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_ZP_X,    [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ZERO_PAGE_X, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_ABS,     [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ABSOLUTE, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_ABS_X,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ABSOLUTE_X, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_ABS_Y,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ABSOLUTE_Y, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_IND_X,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(INDIRECT_X, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_IND_Y,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(INDIRECT_Y, LOGICAL_OPERATION::OR, cycles, memory);}},
        //ORA
        {INSTRUCTIONS::INS_EOR_IM,      [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(IMMEDIATE, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_ZP,      [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ZERO_PAGE, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_ZP_X,    [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ZERO_PAGE_X, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_ABS,     [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ABSOLUTE, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_ABS_X,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ABSOLUTE_X, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_ABS_Y,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ABSOLUTE_Y, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_IND_X,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(INDIRECT_X, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_IND_Y,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(INDIRECT_Y, LOGICAL_OPERATION::XOR, cycles, memory);}},
        //BIT
        {INSTRUCTIONS::INS_BIT_ZP,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ZERO_PAGE, LOGICAL_OPERATION::BIT, cycles, memory);}},
        {INSTRUCTIONS::INS_BIT_ABS,   [this](int32_t& cycles, Memory& memory) { PerformLogicalOnAccumulator(ABSOLUTE, LOGICAL_OPERATION::BIT, cycles, memory);}},
        /////////////////////////////////// LOGICAL OPERATIONS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        ////////////////////////////////// JUMP INSTRUCTION IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_JSR,         [this](int32_t& cycles, Memory& memory) {
            uint16_t absoluteAddress = Fetch16Bits(cycles, memory);
            StackPush16Bits(cycles, memory, PC - 1);
            PC = absoluteAddress;
            cycles--;
        }},
        {INSTRUCTIONS::INS_RTS,         [this](int32_t& cycles, Memory& memory) { PC = StackPop16Bits(cycles, memory) + 1; cycles -= 3; }},
        {INSTRUCTIONS::INS_JMP_ABS,         [this](int32_t& cycles, Memory& memory) { PC = getAbsoluteAddress(cycles, memory); }},
        {INSTRUCTIONS::INS_JMP_IND,         [this](int32_t& cycles, Memory& memory) { PC = Read16Bits(cycles, memory, getAbsoluteAddress(cycles, memory)); }},
        ////////////////////////////////// JUMP INSTRUCTION IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// INCREMENT INSTRUCTION IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_INX,   [this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(IMPLIED_X, MATH_OPERATION::INCREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_INY,   [this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(IMPLIED_Y, MATH_OPERATION::INCREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_DEX,   [this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(IMPLIED_X, MATH_OPERATION::DECREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_DEY,   [this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(IMPLIED_Y, MATH_OPERATION::DECREMENT, cycles, memory);}},

        {INSTRUCTIONS::INS_INC_ZP,   [this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(ZERO_PAGE, MATH_OPERATION::INCREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_INC_ZP_X, [this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(ZERO_PAGE_X, MATH_OPERATION::INCREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_INC_ABS,  [this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(ABSOLUTE, MATH_OPERATION::INCREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_INC_ABS_X,[this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(ABSOLUTE_X, MATH_OPERATION::INCREMENT, cycles, memory);}},

        {INSTRUCTIONS::INS_DEC_ZP,   [this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(ZERO_PAGE, MATH_OPERATION::DECREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_DEC_ZP_X, [this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(ZERO_PAGE_X, MATH_OPERATION::DECREMENT, cycles, memory); }},
        {INSTRUCTIONS::INS_DEC_ABS,  [this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(ABSOLUTE, MATH_OPERATION::DECREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_DEC_ABS_X,[this](int32_t& cycles, Memory& memory) { IncrementDecrementValue(ABSOLUTE_X, MATH_OPERATION::DECREMENT, cycles, memory);}},
        ////////////////////////////////// INCREMENT INSTRUCTION IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// BRANCH INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_BEQ,[this](int32_t& cycles, Memory& memory) { BranchIf(cycles, memory, P.Z, true); }},
        {INSTRUCTIONS::INS_BNE,[this](int32_t& cycles, Memory& memory) { BranchIf(cycles, memory, P.Z, false); }},
        {INSTRUCTIONS::INS_BMI,[this](int32_t& cycles, Memory& memory) { BranchIf(cycles, memory, P.N, true); }},
        {INSTRUCTIONS::INS_BPL,[this](int32_t& cycles, Memory& memory) { BranchIf(cycles, memory, P.N, false); }},
        {INSTRUCTIONS::INS_BCS,[this](int32_t& cycles, Memory& memory) { BranchIf(cycles, memory, P.C, true); }},
        {INSTRUCTIONS::INS_BCC,[this](int32_t& cycles, Memory& memory) { BranchIf(cycles, memory, P.C, false); }},
        {INSTRUCTIONS::INS_BVS,[this](int32_t& cycles, Memory& memory) { BranchIf(cycles, memory, P.V, true); }},
        {INSTRUCTIONS::INS_BVC,[this](int32_t& cycles, Memory& memory) { BranchIf(cycles, memory, P.V, false); }},
        ////////////////////////////////// BRANCH INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// SET/CLEAR FLAGS INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_CLC,[this](int32_t& cycles, Memory& memory) { P.C = 0; cycles--; }},
        {INSTRUCTIONS::INS_SEC,[this](int32_t& cycles, Memory& memory) { P.C = 1; cycles--; }},
        {INSTRUCTIONS::INS_CLD,[this](int32_t& cycles, Memory& memory) { P.D = 0; cycles--; }},
        {INSTRUCTIONS::INS_SED,[this](int32_t& cycles, Memory& memory) { P.D = 1; cycles--; }},
        {INSTRUCTIONS::INS_CLI,[this](int32_t& cycles, Memory& memory) { P.I = 0; cycles--; }},
        {INSTRUCTIONS::INS_SEI,[this](int32_t& cycles, Memory& memory) { P.I = 1; cycles--; }},
        {INSTRUCTIONS::INS_CLV,[this](int32_t& cycles, Memory& memory) { P.V = 0; cycles--; }},
        ////////////////////////////////// SET/CLEAR FLAGS INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// ADD/SUBTRACT WITH CARRY INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_ADC_IM,   [this](int32_t& cycles, Memory& memory) { PerformAddSubtractOnAccumulator(IMMEDIATE, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_ZP,   [this](int32_t& cycles, Memory& memory) { PerformAddSubtractOnAccumulator(ZERO_PAGE, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_ZP_X, [this](int32_t& cycles, Memory& memory) { PerformAddSubtractOnAccumulator(ZERO_PAGE_X, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_ABS,  [this](int32_t& cycles, Memory& memory) { PerformAddSubtractOnAccumulator(ABSOLUTE, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_ABS_X,[this](int32_t& cycles, Memory& memory) { PerformAddSubtractOnAccumulator(ABSOLUTE_X, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_ABS_Y,[this](int32_t& cycles, Memory& memory) { PerformAddSubtractOnAccumulator(ABSOLUTE_Y, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_IND_X,[this](int32_t& cycles, Memory& memory) { PerformAddSubtractOnAccumulator(INDIRECT_X, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_IND_Y,[this](int32_t& cycles, Memory& memory) { PerformAddSubtractOnAccumulator(INDIRECT_Y, MATH_OPERATION::ADD, cycles, memory); }},
        ////////////////////////////////// ADD/SUBTRACT WITH CARRY INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// COMPARE WITH ACCUMULATOR INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_CMP_IM,   [this](int32_t& cycles, Memory& memory) { CompareWithRegister(IMMEDIATE, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_ZP,   [this](int32_t& cycles, Memory& memory) { CompareWithRegister(ZERO_PAGE, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_ZP_X, [this](int32_t& cycles, Memory& memory) { CompareWithRegister(ZERO_PAGE_X, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_ABS,  [this](int32_t& cycles, Memory& memory) { CompareWithRegister(ABSOLUTE, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_ABS_X,[this](int32_t& cycles, Memory& memory) { CompareWithRegister(ABSOLUTE_X, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_ABS_Y,[this](int32_t& cycles, Memory& memory) { CompareWithRegister(ABSOLUTE_Y, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_IND_X,[this](int32_t& cycles, Memory& memory) { CompareWithRegister(INDIRECT_X, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_IND_Y,[this](int32_t& cycles, Memory& memory) { CompareWithRegister(INDIRECT_Y, cycles, memory, A); }},
        ////////////////////////////////// COMPARE WITH ACCUMULATOR INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// COMPARE WITH X REGISTER INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_CPX_IM,   [this](int32_t& cycles, Memory& memory) { CompareWithRegister(IMMEDIATE, cycles, memory, X); }},
        {INSTRUCTIONS::INS_CPX_ZP,   [this](int32_t& cycles, Memory& memory) { CompareWithRegister(ZERO_PAGE, cycles, memory, X); }},
        {INSTRUCTIONS::INS_CPX_ABS,  [this](int32_t& cycles, Memory& memory) { CompareWithRegister(ABSOLUTE, cycles, memory, X); }},
        ////////////////////////////////// COMPARE WITH X REGISTER INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// COMPARE WITH Y REGISTER INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_CPY_IM,   [this](int32_t& cycles, Memory& memory) { CompareWithRegister(IMMEDIATE, cycles, memory, Y); }},
        {INSTRUCTIONS::INS_CPY_ZP,   [this](int32_t& cycles, Memory& memory) { CompareWithRegister(ZERO_PAGE, cycles, memory, Y); }},
        {INSTRUCTIONS::INS_CPY_ABS,  [this](int32_t& cycles, Memory& memory) { CompareWithRegister(ABSOLUTE, cycles, memory, Y); }},
        ////////////////////////////////// COMPARE WITH Y REGISTER INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// SYSTEM FUNCTIONS INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_NOP,[this](int32_t& cycles, Memory& memory) { cycles--; }},
        ////////////////////////////////// SYSTEM FUNCTIONS INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
    };
}
