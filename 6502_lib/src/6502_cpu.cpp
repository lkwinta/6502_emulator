//
// Created by Lukasz on 25.07.2022.
//
#include "6502_cpu.h"

void MOS6502::CPU::Reset(Memory& memory) {
    PC = 0xFFFC;

    S = 0xFF;
    P.C = P.Z = P.I = P.D = P.B = P.V = P.N = 0;
    A = X = Y = 0;

    memory.Initialise();
}

MOS6502::CPU::CPU() {
    fillInstructionsLookupTable();
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
    uint8_t value = Read8Bits(cycles, memory, stackLocation + S);
    S += 1;
    return value;
}

uint16_t MOS6502::CPU::StackPop16Bits(int32_t &cycles, Memory& memory) {
    uint16_t value = Read16Bits(cycles, memory, stackLocation + S + 1);
    S += 2;
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

void MOS6502::CPU::LDSetStatus(uint8_t& reg){
    P.Z = (reg == 0);
    P.N = (reg & 0b10000000) > 0;
}

void MOS6502::CPU::LoadRegister(ADDRESSING_MODES mode, int32_t& cycles, const Memory& memory, uint8_t& reg){
    switch(mode){
        case IMMEDIATE: {
            reg = Fetch8Bits(cycles, memory);
            break;
        }
        case ZERO_PAGE: {
            reg = Read8Bits(cycles, memory, getZeroPageAddress(cycles, memory));
            break;
        }
        case ZERO_PAGE_X: {
            reg = Read8Bits(cycles, memory, getZeroPageAddressX(cycles, memory));
            break;
        }
        case ZERO_PAGE_Y: {
            reg = Read8Bits(cycles, memory, getZeroPageAddressY(cycles, memory));
            break;
        }
        case ABSOLUTE: {
            reg = Read8Bits(cycles, memory, getAbsoluteAddress(cycles, memory));
            break;
        }
        case ABSOLUTE_X: {
            reg = Read8Bits(cycles, memory, getAbsoluteAddressX(cycles, memory, true));
            break;
        }
        case ABSOLUTE_Y: {
            reg = Read8Bits(cycles, memory, getAbsoluteAddressY(cycles, memory, true));
            break;
        }
        case INDIRECT_X: {
            reg = Read8Bits(cycles, memory, getIndirectIndexedAddressX(cycles, memory));
            break;
        }
        case INDIRECT_Y: {
            reg = Read8Bits(cycles, memory, getIndexedIndirectAddressY(cycles, memory, true));
            break;
        }
        default: {
            printf("Unhandled load addressing mode: %d", mode);
            return;
        }
    }
    LDSetStatus(reg);
}

void MOS6502::CPU::StoreRegister(ADDRESSING_MODES mode, int32_t &cycles, Memory &memory, uint8_t &reg) {
    switch(mode) {
        case ZERO_PAGE: {
            Write8Bits(cycles, memory, getZeroPageAddress(cycles, memory), reg);
            break;
        }
        case ZERO_PAGE_X: {
            Write8Bits(cycles, memory, getZeroPageAddressX(cycles, memory), reg);
            break;
        }
        case ZERO_PAGE_Y: {
            Write8Bits(cycles, memory, getZeroPageAddressY(cycles, memory), reg);
            break;
        }
        case ABSOLUTE: {
            Write8Bits(cycles, memory, getAbsoluteAddress(cycles, memory), reg);
            break;
        }
        case ABSOLUTE_X: {
            Write8Bits(cycles, memory, getAbsoluteAddressX(cycles, memory, false), reg);
            cycles--; //write to absolute
            break;
        }
        case ABSOLUTE_Y: {
            Write8Bits(cycles, memory, getAbsoluteAddressY(cycles, memory, false), reg);
            cycles--; //write to absolute
            break;
        }
        case INDIRECT_X: {
            Write8Bits(cycles, memory, getIndirectIndexedAddressX(cycles, memory), reg);
            break;
        }
        case INDIRECT_Y: {
            Write8Bits(cycles, memory, getIndexedIndirectAddressY(cycles, memory, false), reg);
            cycles--; //write to absolute
            break;
        }
        default: {
            printf("Unhandled store addressing mode: %d", mode);
            return;
        }
    }
}

int32_t MOS6502::CPU::Execute(int32_t cycles, Memory& memory){
    int32_t totalCycles = cycles;

    int32_t c = 2;
    uint16_t firstInstructionAddress = Fetch16Bits(c, memory);
    PC = firstInstructionAddress;

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
