//
// Created by Lukasz on 25.07.2022.
//
#include <stdexcept>
#include "6502_cpu.h"

void MOS6502::CPU::Reset(int32_t& cycles, Memory& memory) {
    PC = 0xFFFC;

    S = 0xFF;
    P.C = P.Z = P.I = P.D = P.B = P.V = P.N = 0;
    A = X = Y = 0;

    uint16_t firstInstructionAddress = Fetch16Bits(cycles, memory);
    PC = firstInstructionAddress;
    cycles -= 5;
}

MOS6502::CPU::CPU() {
    fillInstructionsLookupTable();
}

void MOS6502::CPU::LoadProgram(Memory &memory, const uint8_t *program, uint8_t programSize) {
    memory.Initialise();
    memory[0xFFFC] = program[0];
    memory[0xFFFD] = program[1];

    uint16_t programAddress = (program[1] << 8) + program[0];

    for(uint16_t i = 2; i < programSize; i++){
        memory[programAddress + i - 2] = program[i];
    }
}

void MOS6502::CPU::Setup(Memory &memory, uint16_t resetVectorValue) {
    memory.Initialise();
    memory[0xFFFC] = resetVectorValue & 0xFF;
    memory[0xFFFD] = resetVectorValue >> 8;
}

uint16_t MOS6502::CPU::Fetch8Bits(int32_t& cycles, const Memory& memory){
    uint8_t byte = memory[PC];
    PC++;
    cycles--;
    return byte;
}

uint16_t MOS6502::CPU::Fetch16Bits(int32_t& cycles, const Memory& memory){
    uint8_t lowByte = memory.Data[PC];
    PC++;
    uint8_t highByte = memory.Data[PC];
    PC++;

    cycles -= 2;

    uint16_t result = lowByte;
    result |= (highByte << 8);

    return result;
}

uint8_t MOS6502::CPU::Read8Bits(int32_t& cycles, const Memory& memory, uint16_t address){
    uint8_t byte = memory.Data[address];
    cycles--;
    return byte;
}

uint16_t MOS6502::CPU::Read16Bits(int32_t& cycles, const Memory& memory, uint16_t address){
    uint8_t lowByte = memory.Data[address];
    uint8_t highByte = memory.Data[address + 1];

    cycles -= 2;

    uint16_t result = lowByte;
    result |= (highByte << 8);

    return result;
}

void MOS6502::CPU::Write8Bits(int32_t& cycles, Memory& memory, uint16_t address, uint8_t value){
    memory[address] = value;
    cycles--;
}

void MOS6502::CPU::Write16Bits(int32_t& cycles, Memory& memory, uint16_t address, uint16_t value){
    memory[address] = (value & 0xFF);
    memory[address + 1] = (value >> 8);
    cycles -= 2;
}

void MOS6502::CPU::StackPush8Bits(int32_t &cycles, MOS6502::Memory &memory, uint8_t value) {
    Write8Bits(cycles, memory, stackLocation + S, value);
    S -= 1;
}

void MOS6502::CPU::StackPush16Bits(int32_t &cycles, MOS6502::Memory &memory, uint16_t value) {
    Write16Bits(cycles, memory, stackLocation + S - 1, value);
    S -= 2;
}

uint8_t MOS6502::CPU::StackPop8Bits(int32_t &cycles, Memory& memory) {
    S += 1;
    uint8_t value = Read8Bits(cycles, memory, stackLocation + S);
    return value;
}

uint16_t MOS6502::CPU::StackPop16Bits(int32_t &cycles, Memory& memory) {
    S += 2;
    uint16_t value = Read16Bits(cycles, memory, stackLocation + S - 1);
    return value;
}


uint8_t MOS6502::CPU::getZeroPageAddress(int32_t& cycles, const Memory &memory){
    return Fetch8Bits(cycles, memory);
}

uint8_t MOS6502::CPU::getZeroPageAddressX(int32_t &cycles, const Memory &memory) {
    cycles--; // add X register to address
    return getZeroPageAddress(cycles, memory) + X;
}

uint8_t MOS6502::CPU::getZeroPageAddressY(int32_t &cycles, const Memory &memory) {
    cycles--; // add Y register to address
    return getZeroPageAddress(cycles, memory) + Y;
}

uint16_t MOS6502::CPU::getAbsoluteAddress(int32_t& cycles, const Memory& memory){
    return Fetch16Bits(cycles, memory);
}

uint16_t MOS6502::CPU::getAbsoluteAddressX(int32_t& cycles, const Memory& memory, bool checkPageCrossing){
    uint16_t absoluteAddress = getAbsoluteAddress(cycles, memory);
    if(checkPageCrossing &&(absoluteAddress & 0xFF) + X > 0xFF)
        cycles--; // page crossed
    return absoluteAddress + X;
}

uint16_t MOS6502::CPU::getAbsoluteAddressY(int32_t& cycles, const Memory& memory, bool checkPageCrossing){
    uint16_t absoluteAddress = getAbsoluteAddress(cycles, memory);
    if(checkPageCrossing &&(absoluteAddress & 0xFF) + Y > 0xFF)
        cycles--; // page crossed
    return absoluteAddress + Y;
}

uint16_t MOS6502::CPU::getIndirectIndexedAddressX(int32_t &cycles, const MOS6502::Memory &memory) {
    cycles--; // add X register to address
    return Read16Bits(cycles, memory, getZeroPageAddress(cycles, memory) + X);
}

uint16_t MOS6502::CPU::getIndexedIndirectAddressY(int32_t &cycles, const MOS6502::Memory &memory, bool checkPageCrossing) {
    uint16_t targetAddress = Read16Bits(cycles, memory, Fetch8Bits(cycles, memory));
    if(checkPageCrossing && (targetAddress & 0xFF) + Y >= 0xFF)
        cycles--; //page crossed
    return targetAddress + Y;
}

void MOS6502::CPU::SetStatusNZ(uint8_t& reg){
    P.Z = (reg == 0);
    P.N = (reg & 0b10000000) != 0;
}

uint16_t MOS6502::CPU::GetAddress(ADDRESSING_MODES mode, int32_t& cycles, const Memory& memory, bool checkPageCrossing){
    uint16_t address;

    switch(mode){

        case ZERO_PAGE: {
            address = getZeroPageAddress(cycles, memory);
            break;
        }
        case ZERO_PAGE_X: {
            address = getZeroPageAddressX(cycles, memory);
            break;
        }
        case ZERO_PAGE_Y: {
            address = getZeroPageAddressY(cycles, memory);
            break;
        }
        case ABSOLUTE: {
            address = getAbsoluteAddress(cycles, memory);
            break;
        }
        case ABSOLUTE_X: {
            address = getAbsoluteAddressX(cycles, memory, checkPageCrossing);
            break;
        }
        case ABSOLUTE_Y: {
            address = getAbsoluteAddressY(cycles, memory, checkPageCrossing);
            break;
        }
        case INDIRECT_X: {
            address = getIndirectIndexedAddressX(cycles, memory);
            break;
        }
        case INDIRECT_Y: {
            address = getIndexedIndirectAddressY(cycles, memory, checkPageCrossing);
            break;
        }
        default: {
            printf("Unhandled load addressing mode: %d", mode);
            throw std::runtime_error("Unhandled load addressing mode");
        }
    }

    return address;
}

void MOS6502::CPU::LoadRegister(ADDRESSING_MODES mode, int32_t& cycles, const Memory& memory, uint8_t& reg){
   if(mode == IMMEDIATE)
       reg = Fetch8Bits(cycles, memory);
   else
       reg = Read8Bits(cycles, memory, GetAddress(mode, cycles, memory, true));
    SetStatusNZ(reg);
}

void MOS6502::CPU::StoreRegister(ADDRESSING_MODES mode, int32_t &cycles, Memory &memory, uint8_t &reg) {
    Write8Bits(cycles, memory, GetAddress(mode, cycles, memory, false), reg);
    if(mode == ABSOLUTE_X || mode == ABSOLUTE_Y || mode == INDIRECT_Y)
        cycles--;
}


void MOS6502::CPU::PerformLogicalOnAccumulator(ADDRESSING_MODES mode, MOS6502::CPU::LOGICAL_OPERATION operation, int32_t &cycles,
                                               MOS6502::Memory &memory) {
    uint8_t value;
    if(mode == IMMEDIATE)
        value = Fetch8Bits(cycles, memory);
    else
        value = Read8Bits(cycles, memory, GetAddress(mode, cycles, memory, true));

    switch (operation) {
        case LOGICAL_OPERATION::AND: {
            A = (A & value);
            SetStatusNZ(A);
            break;
        }
        case LOGICAL_OPERATION::XOR: {
            A = (A ^ value);
            SetStatusNZ(A);
            break;
        }
        case LOGICAL_OPERATION::OR: {
            A = (A | value);
            SetStatusNZ(A);
            break;
        }
        case LOGICAL_OPERATION::BIT: {
            P.Z = ((A & value) == 0);
            P.V = (value & 0b01000000) != 0;
            P.N = (value & 0b10000000) != 0;
            break;
        }
        default: {
            printf("Unhandled operation: %d", operation);
            throw std::runtime_error("Unhandled load addressing mode");
        }
    }
}

void MOS6502::CPU::IncrementDecrementValue(MOS6502::CPU::ADDRESSING_MODES mode,
                                                    MOS6502::CPU::MATH_OPERATION operation, int32_t &cycles,
                                                    MOS6502::Memory &memory) {
    if(mode == IMPLIED_X){
        if(operation == MATH_OPERATION::INCREMENT)
            X++;
        else if(operation == MATH_OPERATION::DECREMENT)
            X--;
        cycles--;
        SetStatusNZ(X);
    } else if (mode == IMPLIED_Y) {
        if(operation == MATH_OPERATION::INCREMENT)
            Y++;
        else if(operation == MATH_OPERATION::DECREMENT)
            Y--;
        cycles--;
        SetStatusNZ(Y);
    } else {
        uint16_t address = GetAddress(mode, cycles, memory, false);
        uint8_t value = Read8Bits(cycles, memory, address);
        if(operation == MATH_OPERATION::INCREMENT)
            value++;
        else if(operation == MATH_OPERATION::DECREMENT)
            value--;

        cycles--;
        if(mode == ABSOLUTE_X)
            cycles--;
        Write8Bits(cycles, memory, address, value);
        SetStatusNZ(value);
    }
}

void MOS6502::CPU::BranchIf(int32_t &cycles, MOS6502::Memory &memory, bool flag, bool expectedState) {
    auto offset = static_cast<int8_t>(Fetch8Bits(cycles, memory));

    if(flag == expectedState){
        cycles--;
        if((PC >> 8) != ((PC + offset) >> 8))
            cycles -= 2; // page crossed
        PC += offset;
    }
}

int32_t MOS6502::CPU::Execute(int32_t cycles, Memory& memory){
    int32_t totalCycles = cycles;

    while(cycles > 0){
        uint8_t instruction = Fetch8Bits(cycles, memory);
        if(instructionsLookupTable.find((INSTRUCTIONS)instruction) != instructionsLookupTable.end())
            instructionsLookupTable[(INSTRUCTIONS)instruction](cycles, memory);
        else {
            printf("Unknown Instruction!");
            return -1;
        }
    }

    return totalCycles - cycles;
}


