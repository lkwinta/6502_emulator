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
#include "Instructions.h"

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

        enum class LOGICAL_OPERATION {
            AND,
            XOR,
            OR,
            BIT
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

        /*returns address basing on addressing mode*/
        uint16_t GetAddress(ADDRESSING_MODES mode, int32_t& cycles, const Memory& memory, bool checkPageCrossing);

        /*Performs logical operation on accumulator*/
        void PerformLogicalOnAccumulator(ADDRESSING_MODES mode, LOGICAL_OPERATION operation, int32_t& cycles, Memory& memory);
        /*Loads register with specified addressing mode*/
        void LoadRegister(ADDRESSING_MODES mode, int32_t& cycles, const Memory& memory, uint8_t& reg);
        /*Stores register in specified address in memory*/
        void StoreRegister(ADDRESSING_MODES mode, int32_t& cycles, Memory& memory, uint8_t& reg);

        /*Fills lookup table array with instructions*/
        void fillInstructionsLookupTable();

        /*stack index 0, stack pointer is added to that index*/
        uint16_t stackLocation = 0x0100;
    public:
        CPU();

        //lookup table for instructions and their functions
        std::map<INSTRUCTIONS, std::function<void(int32_t&, Memory&)>> instructionsLookupTable;

        //loads program to the memory
        static void LoadProgram(Memory& memory, const uint8_t* program, uint8_t programSize);

        /*
         * sets value of the reset vector
         * deprecated (use load program instead)
         */
        static void Setup(Memory& memory, uint16_t resetVectorValue);

        //cycles: 7      |      reset function
        void Reset(int32_t& cycles, Memory& memory);

        /* return number of cycles used */
        int32_t Execute(int32_t cycles, Memory& memory);

        /////////// REGISTERS ///////////
        uint16_t PC{}; //16-bit program counter
        uint8_t S{}; //8-bit stack pointer

        uint8_t A{}; //8-bit accumulator register

        uint8_t X{}; //8-bit X register
        uint8_t Y{}; //8-bit Y register


        union {
            struct {
                uint8_t C : 1; //1-bit carry flag
                uint8_t Z : 1; //1-bit zero flag
                uint8_t I : 1; //1-bit interrupt disable
                uint8_t D : 1; //1-bit decimal mode
                uint8_t B : 1; //1-bit break command FLAG
                uint8_t UNUSED : 1; //1-bit unused flag
                uint8_t V : 1; //1-bit overflow flag
                uint8_t N : 1; //1-bit negative flag
            };
            //8-bit bit field
            //8-bit field Processor status register
            uint8_t PS;
        } P;
        /////////// REGISTERS ///////////
    };
}

#endif //INC_6502_EMULATOR_6502_CPU_H
