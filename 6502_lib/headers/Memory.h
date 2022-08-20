//
// Created by Lukasz on 26.07.2022.
//

#ifndef INC_6502_PROJECT_MEMORY_H
#define INC_6502_PROJECT_MEMORY_H
#include <cstdint>

namespace MOS6502{
    struct Memory {
        // max memory amount (max possible address reachable)
        static constexpr uint32_t MAX_MEM = 0xFFFF;

        //byte array of MAX_MEM size
        uint8_t Data[MAX_MEM];

        //initialise memory
        void Initialise(){
            for(uint8_t& i : Data){
                i = 0;
            }
        }

        /*Reads one byte from an address*/
        uint8_t operator[](uint32_t address) const {
            return (this, Data[address]);
        }

        /*Writes one byte to an address*/
        uint8_t& operator[](uint32_t address) {
            return (this, Data[address]);
        }

        void LoadProgram(const uint8_t *program, uint8_t programSize) {
            Initialise();
            Data[0xFFFC] = program[0];
            Data[0xFFFD] = program[1];

            uint16_t programAddress = (program[1] << 8) + program[0];

            for(uint16_t i = 2; i < programSize; i++){
                Data[programAddress + i - 2] = program[i];
            }
        }
    };
}

#endif //INC_6502_PROJECT_MEMORY_H
