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
    };
}

#endif //INC_6502_PROJECT_MEMORY_H
