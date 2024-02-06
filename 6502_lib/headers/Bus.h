//
// Created by Lukasz on 26.07.2022.
//

#ifndef INC_6502_PROJECT_BUS_H
#define INC_6502_PROJECT_BUS_H
#include <cstdint>

enum LAYOUT {
    RAM_ONLY,
    NES
};

namespace MOS6502{
    struct Bus {
        // max memory amount (max possible address reachable)
        static constexpr uint32_t MAX_MEM = 0xFFFF;

        //byte array of MAX_MEM size
        uint8_t* RAM;
        //
        uint32_t RAM_SIZE = 0;

        //initialise memory
        void Initialise(){
            for(size_t i = 0; i < RAM_SIZE; i++){
                RAM[i] = 0;
            }
        }


        Bus(LAYOUT layout = RAM_ONLY){
            switch(layout){
                case RAM_ONLY:
                    RAM = new uint8_t[MAX_MEM];
                    RAM_SIZE = MAX_MEM;
                    break;
                case NES:
                    break;
            }
        }

        ~Bus() {
            delete[] RAM;
        }

        /*Reads one byte from an address*/
        uint8_t operator[](uint32_t address) const {
            return (this, RAM[address]);
        }

        /*Writes one byte to an address*/
        uint8_t& operator[](uint32_t address) {
            return (this, RAM[address]);
        }

        void LoadProgram(const uint8_t *program, uint8_t programSize) {
            Initialise();
            RAM[0xFFFC] = program[0];
            RAM[0xFFFD] = program[1];

            uint16_t programAddress = (program[1] << 8) + program[0];

            for(uint16_t i = 2; i < programSize; i++){
                RAM[programAddress + i - 2] = program[i];
            }
        }
    };
}

#endif //INC_6502_PROJECT_BUS_H
