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

namespace MOS6502 {
    class CPU {
    private:
        /* Stores possible addressing modes */
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

        /*return 8-bit zero-page address*/
        uint8_t getZeroPageAddress(int32_t& cycles, const Memory &memory);
        /*return 8-bit zero-page address with an X offset*/
        uint8_t getZeroPageAddressX(int32_t& cycles, const Memory& memory);
        /*return 8-bit zero-page address with an Y offset*/
        uint8_t getZeroPageAddressY(int32_t& cycles, const Memory& memory);
        /*return 16-bit absolute address*/
        uint16_t getAbsoluteAddress(int32_t& cycles, const Memory& memory);
        /*return 16-bit absolute address with an X offset*/
        uint16_t getAbsoluteAddressX(int32_t& cycles, const Memory& memory, bool checkPageCrossing);
        /*return 16-bit absolute address with an Y offset*/
        uint16_t getAbsoluteAddressY(int32_t& cycles, const Memory& memory, bool checkPageCrossing);
        /*return 16-bit address (address = memory[16-bit value + X]) */
        uint16_t getIndirectIndexedAddressX(int32_t& cycles, const Memory& memory);
        /*return 16-bit address (address = memory[8-bit value] + X) */
        uint16_t getIndexedIndirectAddressY(int32_t& cycles, const Memory& memory, bool checkPageCrossing);

        /* Fetches 8-bits (1 byte) from memory (changes program counter)*/
        uint16_t Fetch8Bits(int32_t& cycles, const Memory& memory);
        /* Fetches 16-bits (2 bytes) from memory (changes program counter)*/
        uint16_t Fetch16Bits(int32_t& cycles, const Memory& memory);

        /* Reads 8-bits (1 byte) from memory from address*/
        static uint8_t Read8Bits(int32_t& cycles, const Memory& memory, uint16_t address);
        /* Reads 16-bits (2 bytes) from memory from address (little endian)*/
        static uint16_t Read16Bits(int32_t& cycles, const Memory& memory, uint16_t address);

        /*Writes 16 bits (2 bytes) to an address with little endian convention*/
        static void Write8Bits(int32_t &cycles, Memory &memory, uint16_t address, uint8_t value);
        /*Writes 16 bits (2 bytes) to an address with little endian convention*/
        static void Write16Bits(int32_t &cycles, Memory &memory, uint16_t address, uint16_t value);

        /*push 8-bit value on the stack | 1 cycle*/
        void StackPush8Bits(int32_t& cycles, Memory& memory, uint8_t value);
        /*push 16-bit value on the stack | 2 cycle*/
        void StackPush16Bits(int32_t& cycles, Memory& memory, uint16_t value);

        /*pop 8-bit value from the stack*/
        uint8_t StackPop8Bits(int32_t& cycles, Memory& memory);
        /*pop 16-bit value from the stack*/
        uint16_t StackPop16Bits(int32_t& cycles, Memory& memory);

        /*Sets N and Z flags of processor status after loading a register*/
        void LDSetStatus(uint8_t& reg);

        /*Loads register with specified addressing mode*/
        void LoadRegister(ADDRESSING_MODES mode, int32_t& cycles, const Memory& memory, uint8_t& reg);
        /*Stores register in specified address in memory*/
        void StoreRegister(ADDRESSING_MODES mode, int32_t& cycles, Memory& memory, uint8_t& reg);

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

        /*Fills lookup table array with instructions*/
        void fillInstructionsLookupTable();

        /*stack index 0, stack pointer is added to that index*/
        uint16_t stackLocation = 0x0100;
    public:

        //contains possible instructions
        enum INSTRUCTIONS : uint8_t {
            //cycles: 2  |    args: 8-bit value
            INS_LDA_IM = 0xA9,
            //cycles: 3  |    args: 8-bit zero-page address
            INS_LDA_ZP = 0xA5,
            //cycles: 4  |    args: 8-bit zero-page address, X register offset
            INS_LDA_ZP_X = 0xB5,
            //cycles: 4  |    args: 16-bit absolute address (little endian)
            INS_LDA_ABS = 0xAD,
            //cycles: 4-5|    args: 16-bit absolute address (little endian), X register offset
            INS_LDA_ABS_X = 0xBD,
            //cycles: 4-5|    args: 16-bit absolute address (little endian), Y register offset
            INS_LDA_ABS_Y = 0xB9,
            //cycles: 6  |    args: (8-bit zero-page address, X register offset)
            INS_LDA_IND_X = 0xA1,
            //cycles: 5-6|    args: (8-bit zero-page address), Y register offset
            INS_LDA_IND_Y = 0xB1,

            //cycles: 2  |    args: 8-bit value
            INS_LDX_IM = 0xA2,
            //cycles: 3  |    args: 8-bit zero-page address
            INS_LDX_ZP = 0xA6,
            //cycles: 4  |    args: 8-bit zero-page address, Y register offset
            INS_LDX_ZP_Y = 0xB6,
            //cycles: 4  |    args: 16-bit absolute address (little endian)
            INS_LDX_ABS = 0xAE,
            //cycles: 4-5|    args: 16-bit absolute address (little endian), Y register offset
            INS_LDX_ABS_Y = 0xBE,

            //cycles: 2  |    args: 8-bit value
            INS_LDY_IM = 0xA0,
            //cycles: 3  |    args: 8-bit zero-page address
            INS_LDY_ZP = 0xA4,
            //cycles: 4  |    args: 8-bit zero-page address, X register offset
            INS_LDY_ZP_X = 0xB4,
            //cycles: 4  |    args: 16-bit absolute address (little endian)
            INS_LDY_ABS = 0xAC,
            //cycles: 4-5|    args: 16-bit absolute address (little endian), Y register offset
            INS_LDY_ABS_X = 0xBC,

            //cycles: 3  |    args: 8-bit zero-page address
            INS_STA_ZP = 0x85,
            //cycles: 4  |    args: 8-bit zero-page address, X register offset
            INS_STA_ZP_X = 0x95,
            //cycles: 4  |    args: 16-bit absolute address (little endian)
            INS_STA_ABS = 0x8D,
            //cycles: 5  |    args: 16-bit absolute address (little endian), X register offset
            INS_STA_ABS_X = 0x9D,
            //cycles: 5  |    args: 16-bit absolute address (little endian), Y register offset
            INS_STA_ABS_Y = 0x99,
            //cycles: 6  |    args: (8-bit zero-page address, X register offset)
            INS_STA_IND_X = 0x81,
            //cycles: 6  |    args: (8-bit zero-page address), Y register offset
            INS_STA_IND_Y = 0x91,

            //cycles: 3  |    args: 8-bit zero-page address
            INS_STX_ZP = 0x86,
            //cycles: 4  |    args: 8-bit zero-page address, X register offset
            INS_STX_ZP_Y = 0x96,
            //cycles: 4  |    args: 16-bit absolute address (little endian)
            INS_STX_ABS = 0x8E,

            //cycles: 3  |    args: 8-bit zero-page address
            INS_STY_ZP = 0x84,
            //cycles: 4  |    args: 8-bit zero-page address, X register offset
            INS_STY_ZP_X = 0x94,
            //cycles: 4  |    args: 16-bit absolute address (little endian)
            INS_STY_ABS = 0x8C,

            //cycles: 6  |    args: 16-bit absolute address (little endian)
            INS_JSR = 0x20,
            //cycles: 6  |    args: none (Implied)
            INS_RTS = 0x60,
            //cycles: 3  |    args: 16-bit absolute address (little endian)
            INS_JMP_ABS = 0x4C,
            //cycles: 5  |    args: (16-bit indirect address (little endian))
            INS_JMP_IND = 0x6C,
        };

        CPU();

        //lookup table for instructions and their functions
        std::map<INSTRUCTIONS, std::function<void(int32_t&, Memory&)>> instructionsLookupTable;

        //reset function
        void Reset(Memory& memory);

        /* return number of cycles used */
        int32_t Execute(int32_t cycles, Memory& memory);

        /////////// REGISTERS ///////////
        uint16_t PC{}; //16-bit program counter
        uint8_t S{}; //8-bit stack pointer

        uint8_t A{}; //8-bit accumulator register

        uint8_t X{}; //8-bit X register
        uint8_t Y{}; //8-bit Y register

        PS P{}; //8-bit field Processor status register
        /////////// REGISTERS ///////////
    };
}

#endif //INC_6502_EMULATOR_6502_CPU_H
