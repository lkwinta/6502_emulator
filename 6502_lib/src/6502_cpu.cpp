//
// Created by Lukasz on 25.07.2022.
//
#include "6502_cpu.h"

void CPU::Reset(Memory& memory) {
    PC = 0xFFFC;
    S = 0x0100;
    P.C = P.Z = P.I = P.D = P.B = P.V = P.N = 0;
    A = X = Y = 0;
    memory.Initialise();
}

uint16_t CPU::Fetch8Bits(int32_t& cycles, Memory& memory){
    uint8_t byte = memory[PC];
    PC++;
    cycles--;
    return byte;
}

uint16_t CPU::Fetch16Bits(int32_t& cycles, Memory& memory){
    uint8_t lowByte = memory.Data[PC];
    PC++;
    uint8_t highByte = memory.Data[PC];
    PC++;

    cycles -= 2;

    uint16_t result = lowByte;
    result |= (highByte << 8);

    return result;
}

uint8_t CPU::Read8Bits(int32_t& cycles, Memory& memory, uint16_t address){
    uint8_t byte = memory.Data[address];
    cycles--;
    return byte;
}

uint16_t CPU::Read16Bits(int32_t& cycles, Memory& memory, uint16_t address){
    uint8_t lowByte = memory.Data[address];
    uint8_t highByte = memory.Data[address + 1];

    cycles -= 2;

    uint16_t result = lowByte;
    result |= (highByte << 8);

    return result;
}

void CPU::LDSetStatus(uint8_t& reg){
    P.Z = (reg == 0);
    P.N = (reg & 0b10000000) > 0;
}

void CPU::LoadRegister(ADDRESSING_MODES mode, int32_t& cycles, Memory& memory, uint8_t& reg){
    switch(mode){
        case IMMEDIATE: {
            reg = Fetch8Bits(cycles, memory);
            break;
        }
        case ZERO_PAGE: {
            reg = Read8Bits(cycles, memory, Fetch8Bits(cycles, memory));
            break;
        }
        case ZERO_PAGE_X: {
            uint8_t zeroPageAddress = Fetch8Bits(cycles, memory);
            zeroPageAddress += X;
            cycles--;
            reg = Read8Bits(cycles, memory, zeroPageAddress);
            break;
        }
        case ZERO_PAGE_Y: {
            uint8_t zeroPageAddress = Fetch8Bits(cycles, memory);
            zeroPageAddress += Y;
            cycles--;
            reg = Read8Bits(cycles, memory, zeroPageAddress);
            break;
        }
        case ABSOLUTE: {
            reg = Read8Bits(cycles, memory, Fetch16Bits(cycles, memory));
            break;
        }
        case ABSOLUTE_X: {
            uint16_t absoluteAddress = Fetch16Bits(cycles, memory);
            uint16_t absoluteAddressX = absoluteAddress + X;
            reg = Read8Bits(cycles, memory, absoluteAddressX);
            //crossing page boundary
            if(absoluteAddressX - absoluteAddress >= 0xFF)
                cycles--;
            break;
        }
        case ABSOLUTE_Y: {
            uint16_t absoluteAddress = Fetch16Bits(cycles, memory);
            uint16_t absoluteAddressY = absoluteAddress + Y;
            reg = Read8Bits(cycles, memory, absoluteAddressY);
            //crossing page boundary
            if(absoluteAddressY - absoluteAddress >= 0xFF)
                cycles--;
            break;
        }
        case INDIRECT_X: {
            uint8_t zeroPageAddress = Fetch8Bits(cycles, memory);
            zeroPageAddress += X;
            cycles--; //add X register value
            reg = Read8Bits(cycles, memory, Read16Bits(cycles, memory, zeroPageAddress));
            LDSetStatus(reg);
            break;
        }
        case INDIRECT_Y: {
            uint16_t targetAddress = Read16Bits(cycles, memory, Fetch8Bits(cycles, memory));
            uint16_t targetAddressY = targetAddress + Y;
            reg = Read8Bits(cycles, memory, targetAddressY);
            //crossing page boundary
            if(targetAddressY - targetAddress >= 0xFF)
                cycles--;
            LDSetStatus(reg);
        }
        default: {
            printf("Unhandled addressing mode: %d", mode);
            break;
        }
    }
    LDSetStatus(reg);
}

/* return number of cycles used */
int32_t CPU::Execute(int32_t cycles, Memory& memory){
    int32_t totalCycles = cycles;

    while(cycles > 0){
        uint8_t instruction = Fetch8Bits(cycles, memory);
        lookupTable[(INSTRUCTIONS)instruction](cycles, memory);
    }

    return totalCycles - cycles;
}