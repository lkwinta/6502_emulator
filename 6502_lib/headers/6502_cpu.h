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

    void LDSetStatus(uint8_t& reg){
        P.Z = (reg == 0);
        P.N = (reg & 0b10000000) > 0;
    }

    enum ADDRESSING_MODES{
        IMMEDIATE,
        ZERO_PAGE,
        ZERO_PAGE_X,
        ZERO_PAGE_Y,
        ABSOLUTE,
        ABSOLUTE_X,
        ABSOLUTE_Y
    };

    /////////////////////////////////// LOAD ACCUMULATOR INSTRUCTIONS OPCODES ///////////////////////////////////////
    static constexpr uint8_t    INS_LDA_IM = 0xA9,
                                INS_LDA_ZP = 0xA5,
                                INS_LDA_ZP_X = 0xB5,
                                INS_LDA_ABS = 0xAD,
                                INS_LDA_ABS_X = 0xBD,
                                INS_LDA_ABS_Y = 0xB9,
                                INS_LDA_IND_X = 0xA1,
                                INS_LDA_IND_Y = 0xB1;
    /////////////////////////////////// LOAD ACCUMULATOR INSTRUCTIONS OPCODES ///////////////////////////////////////

    /////////////////////////////////// LOAD X REGISTER INSTRUCTIONS OPCODES ///////////////////////////////////////
    static constexpr uint8_t    INS_LDX_IM = 0xA2,
                                INS_LDX_ZP = 0xA6,
                                INS_LDX_ZP_Y = 0xB6,
                                INS_LDX_ABS = 0xAE,
                                INS_LDX_ABS_Y = 0xBE;
    /////////////////////////////////// LOAD X REGISTER INSTRUCTIONS OPCODES ///////////////////////////////////////

    /////////////////////////////////// LOAD X REGISTER INSTRUCTIONS OPCODES ///////////////////////////////////////
    static constexpr uint8_t    INS_LDY_IM = 0xA0,
                                INS_LDY_ZP = 0xA4,
                                INS_LDY_ZP_X = 0xB4,
                                INS_LDY_ABS = 0xAC,
                                INS_LDY_ABS_X = 0xBC;
    /////////////////////////////////// LOAD X REGISTER INSTRUCTIONS OPCODES ///////////////////////////////////////

    static constexpr uint8_t INS_JSR = 0x20;

    void LoadRegister(ADDRESSING_MODES mode, int32_t& cycles, Mem& memory, uint8_t& reg){
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
            default: {
                printf("Unhandled addressing mode: %d", mode);
                return;
            }
        }
        LDSetStatus(reg);
    }

    /* @return number of cycles used */
    int32_t Execute(int32_t cycles, Mem& memory){
        int32_t totalCycles = cycles;

        while(cycles > 0){
            uint8_t instruction = Fetch8Bits(cycles, memory);

            switch(instruction){
                /////////////////////////////////// LOAD ACCUMULATOR INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
                case INS_LDA_IM: {
                    LoadRegister(IMMEDIATE, cycles, memory, A);
                    break;
                }
                case INS_LDA_ZP: {
                    LoadRegister(ZERO_PAGE, cycles, memory, A);
                    break;
                }
                case INS_LDA_ZP_X: {
                    LoadRegister(ZERO_PAGE_X, cycles, memory, A);
                    break;
                }
                case INS_LDA_ABS: {
                    LoadRegister(ABSOLUTE, cycles, memory, A);
                    break;
                }
                case INS_LDA_ABS_X: {
                    LoadRegister(ABSOLUTE_X, cycles, memory, A);
                    break;
                }
                case INS_LDA_ABS_Y: {
                    LoadRegister(ABSOLUTE_Y, cycles, memory, A);
                    break;
                }
                case INS_LDA_IND_X: {
                    uint8_t zeroPageAddress = Fetch8Bits(cycles, memory);
                    zeroPageAddress += X;
                    cycles--; //add X register value
                    uint16_t targetAddress = Read16Bits(cycles, memory, zeroPageAddress);
                    A = Read8Bits(cycles, memory, targetAddress);
                    LDSetStatus(A);
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
                    LDSetStatus(A);
                    break;
                }
                /////////////////////////////////// LOAD ACCUMULATOR INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

                /////////////////////////////////// LOAD X REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
                case INS_LDX_IM: {
                    LoadRegister(IMMEDIATE, cycles, memory, X);
                    break;
                }
                case INS_LDX_ZP: {
                    LoadRegister(ZERO_PAGE, cycles, memory, X);
                    break;
                }
                case INS_LDX_ZP_Y: {
                    LoadRegister(ZERO_PAGE_Y, cycles, memory, X);
                    break;
                }
                case INS_LDX_ABS: {
                    LoadRegister(ABSOLUTE, cycles, memory, X);
                    break;
                }
                case INS_LDX_ABS_Y: {
                    LoadRegister(ABSOLUTE_Y, cycles, memory, X);
                    break;
                }
                /////////////////////////////////// LOAD X REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

                /////////////////////////////////// LOAD Y REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////
                case INS_LDY_IM: {
                    LoadRegister(IMMEDIATE, cycles, memory, Y);
                    break;
                }
                case INS_LDY_ZP: {
                    LoadRegister(ZERO_PAGE, cycles, memory, Y);
                    break;
                }
                case INS_LDY_ZP_X: {
                    LoadRegister(ZERO_PAGE_X, cycles, memory, Y);
                    break;
                }
                case INS_LDY_ABS: {
                    LoadRegister(ABSOLUTE, cycles, memory, Y);
                    break;
                }
                case INS_LDY_ABS_X: {
                    LoadRegister(ABSOLUTE_X, cycles, memory, Y);
                    break;
                }
                /////////////////////////////////// LOAD Y REGISTER INSTRUCTIONS IMPLEMENTATION ///////////////////////////////////////

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
