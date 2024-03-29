//
// Created by Lukasz on 26.07.2022.
//

#include "6502_cpu.h"

void MOS6502::CPU::fillInstructionsLookupTable(){
    instructionsLookupTable = {
        /////////////////////////////////// LOAD ACCUMULATOR INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_LDA_IM,      [this](int32_t& cycles, Bus& memory) { LoadRegister(IMMEDIATE, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_ZP,      [this](int32_t& cycles, Bus& memory) { LoadRegister(ZERO_PAGE, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_ZP_X,    [this](int32_t& cycles, Bus& memory) { LoadRegister(ZERO_PAGE_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_ABS,     [this](int32_t& cycles, Bus& memory) { LoadRegister(ABSOLUTE, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_ABS_X,   [this](int32_t& cycles, Bus& memory) { LoadRegister(ABSOLUTE_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_ABS_Y,   [this](int32_t& cycles, Bus& memory) { LoadRegister(ABSOLUTE_Y, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_IND_X,   [this](int32_t& cycles, Bus& memory) { LoadRegister(INDIRECT_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_LDA_IND_Y,   [this](int32_t& cycles, Bus& memory) { LoadRegister(INDIRECT_Y, cycles, memory, A);}},
        /////////////////////////////////// LOAD ACCUMULATOR INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// LOAD X REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_LDX_IM,      [this](int32_t& cycles, Bus& memory) { LoadRegister(IMMEDIATE, cycles, memory, X);}},
        {INSTRUCTIONS::INS_LDX_ZP,      [this](int32_t& cycles, Bus& memory) { LoadRegister(ZERO_PAGE, cycles, memory, X);}},
        {INSTRUCTIONS::INS_LDX_ZP_Y,    [this](int32_t& cycles, Bus& memory) { LoadRegister(ZERO_PAGE_Y, cycles, memory, X);}},
        {INSTRUCTIONS::INS_LDX_ABS,     [this](int32_t& cycles, Bus& memory) { LoadRegister(ABSOLUTE, cycles, memory, X);}},
        {INSTRUCTIONS::INS_LDX_ABS_Y,   [this](int32_t& cycles, Bus& memory) { LoadRegister(ABSOLUTE_Y, cycles, memory, X);}},
        /////////////////////////////////// LOAD X REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// LOAD Y REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_LDY_IM,      [this](int32_t& cycles, Bus& memory) { LoadRegister(IMMEDIATE, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_LDY_ZP,      [this](int32_t& cycles, Bus& memory) { LoadRegister(ZERO_PAGE, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_LDY_ZP_X,    [this](int32_t& cycles, Bus& memory) { LoadRegister(ZERO_PAGE_X, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_LDY_ABS,     [this](int32_t& cycles, Bus& memory) { LoadRegister(ABSOLUTE, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_LDY_ABS_X,   [this](int32_t& cycles, Bus& memory) { LoadRegister(ABSOLUTE_X, cycles, memory, Y);}},
        /////////////////////////////////// LOAD Y REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// STORE A REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_STA_ZP,      [this](int32_t& cycles, Bus& memory) { StoreRegister(ZERO_PAGE, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_ZP_X,    [this](int32_t& cycles, Bus& memory) { StoreRegister(ZERO_PAGE_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_ABS,     [this](int32_t& cycles, Bus& memory) { StoreRegister(ABSOLUTE, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_ABS_X,   [this](int32_t& cycles, Bus& memory) { StoreRegister(ABSOLUTE_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_ABS_Y,   [this](int32_t& cycles, Bus& memory) { StoreRegister(ABSOLUTE_Y, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_IND_X,   [this](int32_t& cycles, Bus& memory) { StoreRegister(INDIRECT_X, cycles, memory, A);}},
        {INSTRUCTIONS::INS_STA_IND_Y,   [this](int32_t& cycles, Bus& memory) { StoreRegister(INDIRECT_Y, cycles, memory, A);}},
        /////////////////////////////////// STORE A REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// STORE X REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_STX_ZP,      [this](int32_t& cycles, Bus& memory) { StoreRegister(ZERO_PAGE, cycles, memory, X);}},
        {INSTRUCTIONS::INS_STX_ZP_Y,     [this](int32_t& cycles, Bus& memory) { StoreRegister(ZERO_PAGE_Y, cycles, memory, X);}},
        {INSTRUCTIONS::INS_STX_ABS,      [this](int32_t& cycles, Bus& memory) { StoreRegister(ABSOLUTE, cycles, memory, X);}},
        /////////////////////////////////// STORE X REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// STORE Y REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_STY_ZP,      [this](int32_t& cycles, Bus& memory) { StoreRegister(ZERO_PAGE, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_STY_ZP_X,    [this](int32_t& cycles, Bus& memory) { StoreRegister(ZERO_PAGE_X, cycles, memory, Y);}},
        {INSTRUCTIONS::INS_STY_ABS,     [this](int32_t& cycles, Bus& memory) { StoreRegister(ABSOLUTE, cycles, memory, Y);}},
        /////////////////////////////////// STORE Y REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// TRANSFER REGISTERS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_TAX,      [this](int32_t& cycles, Bus& memory) { X = A; SetStatusNZ(X); cycles--; }},
        {INSTRUCTIONS::INS_TXA,      [this](int32_t& cycles, Bus& memory) { A = X; SetStatusNZ(A); cycles--; }},
        {INSTRUCTIONS::INS_TAY,      [this](int32_t& cycles, Bus& memory) { Y = A; SetStatusNZ(Y); cycles--; }},
        {INSTRUCTIONS::INS_TYA,      [this](int32_t& cycles, Bus& memory) { A = Y; SetStatusNZ(A); cycles--; }},
        {INSTRUCTIONS::INS_TSX,      [this](int32_t& cycles, Bus& memory) { X = S; SetStatusNZ(X); cycles--; }},
        {INSTRUCTIONS::INS_TXS,      [this](int32_t& cycles, Bus& memory) { S = X; cycles--; }},
        /////////////////////////////////// TRANSFER REGISTERS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        /////////////////////////////////// STACK OPERATIONS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        {INSTRUCTIONS::INS_PHA,      [this](int32_t& cycles, Bus& memory) { StackPush8Bits(cycles, memory, A); cycles--; }},
        {INSTRUCTIONS::INS_PHP,      [this](int32_t& cycles, Bus& memory) { StackPush8Bits(cycles, memory, P.PS | UnusedBitFlag | BreakBitFlag); cycles--; }},
        {INSTRUCTIONS::INS_PLA,      [this](int32_t& cycles, Bus& memory) { A = StackPop8Bits(cycles, memory); cycles -= 2; SetStatusNZ(A);}},
        {INSTRUCTIONS::INS_PLP,      [this](int32_t& cycles, Bus& memory) {
            uint8_t stackPS = StackPop8Bits(cycles, memory);
            stackPS &= ~(UnusedBitFlag | BreakBitFlag);
            P.PS &= (UnusedBitFlag | BreakBitFlag);
            P.PS |= stackPS;
            cycles -= 2;
        }},
        /////////////////////////////////// STACK OPERATIONS INSTRUCTIONS IMPLEMENTATION //////////////////// ///////////////////

        /////////////////////////////////// LOGICAL OPERATIONS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
        //AND
        {INSTRUCTIONS::INS_AND_IM,      [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(IMMEDIATE, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_ZP,      [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ZERO_PAGE, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_ZP_X,    [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ZERO_PAGE_X, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_ABS,     [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ABSOLUTE, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_ABS_X,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ABSOLUTE_X, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_ABS_Y,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ABSOLUTE_Y, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_IND_X,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(INDIRECT_X, LOGICAL_OPERATION::AND, cycles, memory);}},
        {INSTRUCTIONS::INS_AND_IND_Y,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(INDIRECT_Y, LOGICAL_OPERATION::AND, cycles, memory);}},
        //EOR
        {INSTRUCTIONS::INS_ORA_IM,      [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(IMMEDIATE, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_ZP,      [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ZERO_PAGE, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_ZP_X,    [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ZERO_PAGE_X, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_ABS,     [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ABSOLUTE, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_ABS_X,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ABSOLUTE_X, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_ABS_Y,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ABSOLUTE_Y, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_IND_X,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(INDIRECT_X, LOGICAL_OPERATION::OR, cycles, memory);}},
        {INSTRUCTIONS::INS_ORA_IND_Y,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(INDIRECT_Y, LOGICAL_OPERATION::OR, cycles, memory);}},
        //ORA
        {INSTRUCTIONS::INS_EOR_IM,      [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(IMMEDIATE, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_ZP,      [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ZERO_PAGE, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_ZP_X,    [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ZERO_PAGE_X, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_ABS,     [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ABSOLUTE, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_ABS_X,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ABSOLUTE_X, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_ABS_Y,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ABSOLUTE_Y, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_IND_X,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(INDIRECT_X, LOGICAL_OPERATION::XOR, cycles, memory);}},
        {INSTRUCTIONS::INS_EOR_IND_Y,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(INDIRECT_Y, LOGICAL_OPERATION::XOR, cycles, memory);}},
        //BIT
        {INSTRUCTIONS::INS_BIT_ZP,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ZERO_PAGE, LOGICAL_OPERATION::BIT, cycles, memory);}},
        {INSTRUCTIONS::INS_BIT_ABS,   [this](int32_t& cycles, Bus& memory) { PerformLogicalOnAccumulator(ABSOLUTE, LOGICAL_OPERATION::BIT, cycles, memory);}},
        /////////////////////////////////// LOGICAL OPERATIONS INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

        ////////////////////////////////// JUMP INSTRUCTION IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_JSR,         [this](int32_t& cycles, Bus& memory) {
            uint16_t absoluteAddress = Fetch16Bits(cycles, memory);
            StackPush16Bits(cycles, memory, PC - 1);
            PC = absoluteAddress;
            cycles--;
        }},
        {INSTRUCTIONS::INS_RTS,         [this](int32_t& cycles, Bus& memory) { PC = StackPop16Bits(cycles, memory) + 1; cycles -= 3; }},
        {INSTRUCTIONS::INS_JMP_ABS,     [this](int32_t& cycles, Bus& memory) { PC = getAbsoluteAddress(cycles, memory); }},
        {INSTRUCTIONS::INS_JMP_IND,     [this](int32_t& cycles, Bus& memory) {
            uint16_t lsb = Fetch8Bits(cycles, memory);
            uint16_t msb = Fetch8Bits(cycles, memory);

            uint16_t address = (msb << 8) | lsb;

            if (lsb == 0x00FF)
                PC = Read8Bits(cycles,memory, address & 0xFF00 << 8) | Read8Bits(cycles, memory, address);
            else
                PC = Read16Bits(cycles, memory, address);
        }},
        ////////////////////////////////// JUMP INSTRUCTION IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// INCREMENT INSTRUCTION IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_INX,   [this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(IMPLIED_X, MATH_OPERATION::INCREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_INY,   [this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(IMPLIED_Y, MATH_OPERATION::INCREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_DEX,   [this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(IMPLIED_X, MATH_OPERATION::DECREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_DEY,   [this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(IMPLIED_Y, MATH_OPERATION::DECREMENT, cycles, memory);}},

        {INSTRUCTIONS::INS_INC_ZP,   [this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(ZERO_PAGE, MATH_OPERATION::INCREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_INC_ZP_X, [this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(ZERO_PAGE_X, MATH_OPERATION::INCREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_INC_ABS,  [this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(ABSOLUTE, MATH_OPERATION::INCREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_INC_ABS_X,[this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(ABSOLUTE_X, MATH_OPERATION::INCREMENT, cycles, memory);}},

        {INSTRUCTIONS::INS_DEC_ZP,   [this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(ZERO_PAGE, MATH_OPERATION::DECREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_DEC_ZP_X, [this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(ZERO_PAGE_X, MATH_OPERATION::DECREMENT, cycles, memory); }},
        {INSTRUCTIONS::INS_DEC_ABS,  [this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(ABSOLUTE, MATH_OPERATION::DECREMENT, cycles, memory);}},
        {INSTRUCTIONS::INS_DEC_ABS_X,[this](int32_t& cycles, Bus& memory) { IncrementDecrementValue(ABSOLUTE_X, MATH_OPERATION::DECREMENT, cycles, memory);}},
        ////////////////////////////////// INCREMENT INSTRUCTION IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// BRANCH INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_BEQ,[this](int32_t& cycles, Bus& memory) { BranchIf(cycles, memory, P.Z, true); }},
        {INSTRUCTIONS::INS_BNE,[this](int32_t& cycles, Bus& memory) { BranchIf(cycles, memory, P.Z, false); }},
        {INSTRUCTIONS::INS_BMI,[this](int32_t& cycles, Bus& memory) { BranchIf(cycles, memory, P.N, true); }},
        {INSTRUCTIONS::INS_BPL,[this](int32_t& cycles, Bus& memory) { BranchIf(cycles, memory, P.N, false); }},
        {INSTRUCTIONS::INS_BCS,[this](int32_t& cycles, Bus& memory) { BranchIf(cycles, memory, P.C, true); }},
        {INSTRUCTIONS::INS_BCC,[this](int32_t& cycles, Bus& memory) { BranchIf(cycles, memory, P.C, false); }},
        {INSTRUCTIONS::INS_BVS,[this](int32_t& cycles, Bus& memory) { BranchIf(cycles, memory, P.V, true); }},
        {INSTRUCTIONS::INS_BVC,[this](int32_t& cycles, Bus& memory) { BranchIf(cycles, memory, P.V, false); }},
        ////////////////////////////////// BRANCH INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// SET/CLEAR FLAGS INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_CLC,[this](int32_t& cycles, Bus& memory) { P.C = 0; cycles--; }},
        {INSTRUCTIONS::INS_SEC,[this](int32_t& cycles, Bus& memory) { P.C = 1; cycles--; }},
        {INSTRUCTIONS::INS_CLD,[this](int32_t& cycles, Bus& memory) { P.D = 0; cycles--; }},
        {INSTRUCTIONS::INS_SED,[this](int32_t& cycles, Bus& memory) { P.D = 1; cycles--; }},
        {INSTRUCTIONS::INS_CLI,[this](int32_t& cycles, Bus& memory) { P.I = 0; cycles--; }},
        {INSTRUCTIONS::INS_SEI,[this](int32_t& cycles, Bus& memory) { P.I = 1; cycles--; }},
        {INSTRUCTIONS::INS_CLV,[this](int32_t& cycles, Bus& memory) { P.V = 0; cycles--; }},
        ////////////////////////////////// SET/CLEAR FLAGS INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// ADD WITH CARRY INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_ADC_IM,   [this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(IMMEDIATE, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_ZP,   [this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(ZERO_PAGE, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_ZP_X, [this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(ZERO_PAGE_X, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_ABS,  [this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(ABSOLUTE, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_ABS_X,[this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(ABSOLUTE_X, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_ABS_Y,[this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(ABSOLUTE_Y, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_IND_X,[this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(INDIRECT_X, MATH_OPERATION::ADD, cycles, memory); }},
        {INSTRUCTIONS::INS_ADC_IND_Y,[this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(INDIRECT_Y, MATH_OPERATION::ADD, cycles, memory); }},
        ////////////////////////////////// ADD WITH CARRY INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// SUBTRACT WITH CARRY INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_SBC_IM,   [this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(IMMEDIATE, MATH_OPERATION::SUBTRACT, cycles, memory); }},
        {INSTRUCTIONS::INS_SBC_ZP,   [this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(ZERO_PAGE, MATH_OPERATION::SUBTRACT, cycles, memory); }},
        {INSTRUCTIONS::INS_SBC_ZP_X, [this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(ZERO_PAGE_X, MATH_OPERATION::SUBTRACT, cycles, memory); }},
        {INSTRUCTIONS::INS_SBC_ABS,  [this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(ABSOLUTE, MATH_OPERATION::SUBTRACT, cycles, memory); }},
        {INSTRUCTIONS::INS_SBC_ABS_X,[this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(ABSOLUTE_X, MATH_OPERATION::SUBTRACT, cycles, memory); }},
        {INSTRUCTIONS::INS_SBC_ABS_Y,[this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(ABSOLUTE_Y, MATH_OPERATION::SUBTRACT, cycles, memory); }},
        {INSTRUCTIONS::INS_SBC_IND_X,[this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(INDIRECT_X, MATH_OPERATION::SUBTRACT, cycles, memory); }},
        {INSTRUCTIONS::INS_SBC_IND_Y,[this](int32_t& cycles, Bus& memory) { PerformAddSubtractOnAccumulator(INDIRECT_Y, MATH_OPERATION::SUBTRACT, cycles, memory); }},
        ////////////////////////////////// SUBTRACT WITH CARRY INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// COMPARE WITH ACCUMULATOR INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_CMP_IM,   [this](int32_t& cycles, Bus& memory) { CompareWithRegister(IMMEDIATE, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_ZP,   [this](int32_t& cycles, Bus& memory) { CompareWithRegister(ZERO_PAGE, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_ZP_X, [this](int32_t& cycles, Bus& memory) { CompareWithRegister(ZERO_PAGE_X, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_ABS,  [this](int32_t& cycles, Bus& memory) { CompareWithRegister(ABSOLUTE, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_ABS_X,[this](int32_t& cycles, Bus& memory) { CompareWithRegister(ABSOLUTE_X, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_ABS_Y,[this](int32_t& cycles, Bus& memory) { CompareWithRegister(ABSOLUTE_Y, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_IND_X,[this](int32_t& cycles, Bus& memory) { CompareWithRegister(INDIRECT_X, cycles, memory, A); }},
        {INSTRUCTIONS::INS_CMP_IND_Y,[this](int32_t& cycles, Bus& memory) { CompareWithRegister(INDIRECT_Y, cycles, memory, A); }},
        ////////////////////////////////// COMPARE WITH ACCUMULATOR INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// COMPARE WITH X REGISTER INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_CPX_IM,   [this](int32_t& cycles, Bus& memory) { CompareWithRegister(IMMEDIATE, cycles, memory, X); }},
        {INSTRUCTIONS::INS_CPX_ZP,   [this](int32_t& cycles, Bus& memory) { CompareWithRegister(ZERO_PAGE, cycles, memory, X); }},
        {INSTRUCTIONS::INS_CPX_ABS,  [this](int32_t& cycles, Bus& memory) { CompareWithRegister(ABSOLUTE, cycles, memory, X); }},
        ////////////////////////////////// COMPARE WITH X REGISTER INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// COMPARE WITH Y REGISTER INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_CPY_IM,   [this](int32_t& cycles, Bus& memory) { CompareWithRegister(IMMEDIATE, cycles, memory, Y); }},
        {INSTRUCTIONS::INS_CPY_ZP,   [this](int32_t& cycles, Bus& memory) { CompareWithRegister(ZERO_PAGE, cycles, memory, Y); }},
        {INSTRUCTIONS::INS_CPY_ABS,  [this](int32_t& cycles, Bus& memory) { CompareWithRegister(ABSOLUTE, cycles, memory, Y); }},
        ////////////////////////////////// COMPARE WITH Y REGISTER INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// SHIFT LEFT INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_ASL_A,    [this](int32_t& cycles, Bus& memory) { ShiftValue(ACCUMULATOR, MATH_OPERATION::SHIFT_LEFT, cycles, memory);}},
        {INSTRUCTIONS::INS_ASL_ZP,   [this](int32_t& cycles, Bus& memory) { ShiftValue(ZERO_PAGE, MATH_OPERATION::SHIFT_LEFT, cycles, memory);}},
        {INSTRUCTIONS::INS_ASL_ZP_X, [this](int32_t& cycles, Bus& memory) { ShiftValue(ZERO_PAGE_X, MATH_OPERATION::SHIFT_LEFT, cycles, memory);}},
        {INSTRUCTIONS::INS_ASL_ABS,  [this](int32_t& cycles, Bus& memory) { ShiftValue(ABSOLUTE, MATH_OPERATION::SHIFT_LEFT, cycles, memory);}},
        {INSTRUCTIONS::INS_ASL_ABS_X,[this](int32_t& cycles, Bus& memory) { ShiftValue(ABSOLUTE_X, MATH_OPERATION::SHIFT_LEFT, cycles, memory);}},
        ////////////////////////////////// SHIFT LEFT INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// SHIFT RIGHT INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_LSR_A,    [this](int32_t& cycles, Bus& memory) { ShiftValue(ACCUMULATOR, MATH_OPERATION::SHIFT_RIGHT, cycles, memory);}},
        {INSTRUCTIONS::INS_LSR_ZP,   [this](int32_t& cycles, Bus& memory) { ShiftValue(ZERO_PAGE, MATH_OPERATION::SHIFT_RIGHT, cycles, memory);}},
        {INSTRUCTIONS::INS_LSR_ZP_X, [this](int32_t& cycles, Bus& memory) { ShiftValue(ZERO_PAGE_X, MATH_OPERATION::SHIFT_RIGHT, cycles, memory);}},
        {INSTRUCTIONS::INS_LSR_ABS,  [this](int32_t& cycles, Bus& memory) { ShiftValue(ABSOLUTE, MATH_OPERATION::SHIFT_RIGHT, cycles, memory);}},
        {INSTRUCTIONS::INS_LSR_ABS_X,[this](int32_t& cycles, Bus& memory) { ShiftValue(ABSOLUTE_X, MATH_OPERATION::SHIFT_RIGHT, cycles, memory);}},
        ////////////////////////////////// SHIFT RIGHT INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// ROTATE LEFT INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_ROL_A,    [this](int32_t& cycles, Bus& memory) { ShiftValue(ACCUMULATOR, MATH_OPERATION::ROTATE_LEFT, cycles, memory);}},
        {INSTRUCTIONS::INS_ROL_ZP,   [this](int32_t& cycles, Bus& memory) { ShiftValue(ZERO_PAGE, MATH_OPERATION::ROTATE_LEFT, cycles, memory);}},
        {INSTRUCTIONS::INS_ROL_ZP_X, [this](int32_t& cycles, Bus& memory) { ShiftValue(ZERO_PAGE_X, MATH_OPERATION::ROTATE_LEFT, cycles, memory);}},
        {INSTRUCTIONS::INS_ROL_ABS,  [this](int32_t& cycles, Bus& memory) { ShiftValue(ABSOLUTE, MATH_OPERATION::ROTATE_LEFT, cycles, memory);}},
        {INSTRUCTIONS::INS_ROL_ABS_X,[this](int32_t& cycles, Bus& memory) { ShiftValue(ABSOLUTE_X, MATH_OPERATION::ROTATE_LEFT, cycles, memory);}},
        ////////////////////////////////// ROTATE LEFT INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// ROTATE RIGHT INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_ROR_A,    [this](int32_t& cycles, Bus& memory) { ShiftValue(ACCUMULATOR, MATH_OPERATION::ROTATE_RIGHT, cycles, memory);}},
        {INSTRUCTIONS::INS_ROR_ZP,   [this](int32_t& cycles, Bus& memory) { ShiftValue(ZERO_PAGE, MATH_OPERATION::ROTATE_RIGHT, cycles, memory);}},
        {INSTRUCTIONS::INS_ROR_ZP_X, [this](int32_t& cycles, Bus& memory) { ShiftValue(ZERO_PAGE_X, MATH_OPERATION::ROTATE_RIGHT, cycles, memory);}},
        {INSTRUCTIONS::INS_ROR_ABS,  [this](int32_t& cycles, Bus& memory) { ShiftValue(ABSOLUTE, MATH_OPERATION::ROTATE_RIGHT, cycles, memory);}},
        {INSTRUCTIONS::INS_ROR_ABS_X,[this](int32_t& cycles, Bus& memory) { ShiftValue(ABSOLUTE_X, MATH_OPERATION::ROTATE_RIGHT, cycles, memory);}},
        ////////////////////////////////// ROTATE RIGHT INSTRUCTIONS IMPLEMENTATION //////////////////////////////////

        ////////////////////////////////// SYSTEM FUNCTIONS INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
        {INSTRUCTIONS::INS_BRK,[this](int32_t& cycles, Bus& memory) {
            PC++;
            StackPush16Bits(cycles, memory, PC);
            StackPush8Bits(cycles, memory, P.PS | UnusedBitFlag | BreakBitFlag);
            PC = 0xFFFE;
            PC = Fetch16Bits(cycles, memory);
            P.I = true;
            cycles--;
        }},
        {INSTRUCTIONS::INS_NOP,[this](int32_t& cycles, Bus& memory) { cycles--; }},
        {INSTRUCTIONS::INS_RTI,[this](int32_t& cycles, Bus& memory) {
            uint8_t stackPS = StackPop8Bits(cycles, memory);
            stackPS &= ~(UnusedBitFlag | BreakBitFlag);
            P.PS &= (UnusedBitFlag | BreakBitFlag);
            P.PS |= stackPS;
            PC = StackPop16Bits(cycles, memory);
            cycles -= 2;
        }},
        ////////////////////////////////// SYSTEM FUNCTIONS INSTRUCTIONS IMPLEMENTATION //////////////////////////////////
    };
}
