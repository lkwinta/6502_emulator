//
// Created by Lukasz on 26.07.2022.
//

#ifndef INC_6502_PROJECT_MEMORY_H
#define INC_6502_PROJECT_MEMORY_H
#include <cstdint>

struct Memory {
    static constexpr uint32_t MAX_MEM = 0xFFFF;
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

#endif //INC_6502_PROJECT_MEMORY_H
