//
// Created by Lukasz on 25.07.2022.
//

#ifndef INC_6502_EMULATOR_6502_CPU_H
#define INC_6502_EMULATOR_6502_CPU_H
#include <cstdint>
#include <cstdio>

struct Mem {
    static constexpr uint32_t MAX_MEM = 1024 * 64;
    uint8_t Data[MAX_MEM];

    void Initialise(){
        for(uint8_t& i : Data){
            i = 0;
        }
    }

    uint8_t operator[](uint32_t address) const {
        return (this, Data[address]);
    }

    uint8_t& operator[](uint32_t address) {
        return (this, Data[address]);
    }

    void Write16Bits(uint32_t address, uint16_t value){
        Data[address] = (value & 0xFF);
        Data[address + 1] = (value >> 8);
    }
};

struct CPU {
    uint16_t PC; //16-bit program counter
    uint16_t S; //16-bit stack pointer

    uint8_t A; //8-bit accumulator

    uint8_t X; //8-bit X register
    uint8_t Y; //8-bit Y register

    struct PS {
        uint8_t C : 1; //1-bit carry flag
        uint8_t Z : 1; //1-bit zero flag
        uint8_t I : 1; //1-bit interrupt disable
        uint8_t D : 1; //1-bit decimal mode
        uint8_t B : 1; //1-bit break command
        uint8_t V : 1; //1-bit overflow flag
        uint8_t N : 1; //1-bit negative flag
    };

    PS P; //8-bit Processor status

    //reset function
    void Reset(Mem& memory) {
        PC = 0xFFFC;
        S = 0x0100;
        P.C = P.Z = P.I = P.D = P.B = P.V = P.N = 0;
        A = X = Y = 0;
        memory.Initialise();
    }

    uint16_t Fetch8Bits(int32_t& cycles, Mem& memory){
        uint8_t byte = memory[PC];
        PC++;
        cycles--;
        return byte;
    }

    uint16_t Fetch16Bits(int32_t& cycles, Mem& memory){
        uint8_t lowByte = memory.Data[PC];
        PC++;
        uint8_t highByte = memory.Data[PC];
        PC++;

        cycles -= 2;

        uint16_t result = lowByte;
        result |= (highByte << 8);

        return result;
    }

    static uint8_t Read8Bits(int32_t& cycles, Mem& memory, uint16_t address){
        uint8_t byte = memory.Data[address];
        cycles--;
        return byte;
    }

    static uint16_t Read16Bits(int32_t& cycles, Mem& memory, uint16_t address){
        uint8_t lowByte = memory.Data[address];
        uint8_t highByte = memory.Data[address + 1];

        cycles -= 2;

        uint16_t result = lowByte;
        result |= (highByte << 8);

        return result;
    }

    void LDASetStatus(){
        P.Z = (A == 0);
        P.N = (A & 0b10000000) > 0;
    }

    static constexpr uint8_t INS_LDA_IM = 0xA9;
    static constexpr uint8_t INS_LDA_ZP = 0xA5;
    static constexpr uint8_t INS_LDA_ZP_X = 0xB5;
    static constexpr uint8_t INS_LDA_ABS = 0xAD;
    static constexpr uint8_t INS_LDA_ABS_X = 0xBD;
    static constexpr uint8_t INS_LDA_ABS_Y = 0xB9;
    static constexpr uint8_t INS_LDA_IND_X = 0xA1;
    static constexpr uint8_t INS_LDA_IND_Y = 0xB1;

    static constexpr uint8_t INS_JSR = 0x20;

    /* @return number of cycles used */
    int32_t Execute(int32_t cycles, Mem& memory){
        int32_t totalCycles = cycles;

        while(cycles > 0){
            uint8_t instruction = Fetch8Bits(cycles, memory);

            switch(instruction){
                case INS_LDA_IM: {
                    uint8_t value = Fetch8Bits(cycles, memory);
                    A = value;
                    LDASetStatus();
                    break;
                }
                case INS_LDA_ZP: {
                    uint8_t zeroPageAddress = Fetch8Bits(cycles, memory);
                    A = Read8Bits(cycles, memory, zeroPageAddress);
                    LDASetStatus();
                    break;
                }
                case INS_LDA_ZP_X: {
                    uint8_t zeroPageAddress = Fetch8Bits(cycles, memory);
                    zeroPageAddress += X;
                    cycles--; //add X register
                    A = Read8Bits(cycles, memory,zeroPageAddress);
                    LDASetStatus();
                    break;
                }
                case INS_LDA_ABS: {
                    uint16_t absoluteAddress = Fetch16Bits(cycles, memory);
                    A = Read8Bits(cycles, memory, absoluteAddress);
                    LDASetStatus();
                    break;
                }
                case INS_LDA_ABS_X: {
                    uint16_t absoluteAddress = Fetch16Bits(cycles, memory);
                    uint16_t absoluteAddressX = absoluteAddress + X;
                    A = Read8Bits(cycles, memory, absoluteAddressX);
                    //crossing page boundary
                    if(absoluteAddressX - absoluteAddress >= 0xFF)
                        cycles--;
                    LDASetStatus();
                    break;
                }
                case INS_LDA_ABS_Y: {
                    uint16_t absoluteAddress = Fetch16Bits(cycles, memory);
                    uint16_t absoluteAddressY = absoluteAddress + Y;
                    A = Read8Bits(cycles, memory, absoluteAddressY);
                    //crossing page boundary
                    if(absoluteAddressY - absoluteAddress >= 0xFF)
                        cycles--;
                    LDASetStatus();
                    break;
                }
                case INS_LDA_IND_X: {
                    uint8_t zeroPageAddress = Fetch8Bits(cycles, memory);
                    zeroPageAddress += X;
                    cycles--; //add X register value
                    uint16_t targetAddress = Read16Bits(cycles, memory, zeroPageAddress);
                    A = Read8Bits(cycles, memory, targetAddress);
                    LDASetStatus();
                    break;
                }

                case INS_LDA_IND_Y: {
                    uint8_t zeroPageAddress = Fetch8Bits(cycles, memory);
                    uint16_t targetAddress = Read16Bits(cycles, memory, zeroPageAddress);
                    uint16_t targetAddressY = targetAddress + Y;
                    A = Read8Bits(cycles, memory, targetAddressY);
                    //crossing page boundary
                    if(targetAddressY - targetAddress >= 0xFF)
                        cycles--;
                    LDASetStatus();
                    break;
                }

                case INS_JSR:{
                    uint16_t absoluteAddress = Fetch16Bits(cycles, memory);
                    memory.Write16Bits(S,PC - 1 );
                    cycles -= 2;
                    S += 2;
                    PC = absoluteAddress;
                    cycles--;
                    break;
                }
                default: {
                    printf_s("Instruction: 0x%X not handled!\n", instruction);
                    return -1;
                }
            }
        }

        return totalCycles - cycles;
    }
};

#endif //INC_6502_EMULATOR_6502_CPU_H
