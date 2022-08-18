//
// Created by Lukasz on 11.08.2022.
//

#ifndef INC_6502_PROJECT_INSTRUCTIONS_H
#define INC_6502_PROJECT_INSTRUCTIONS_H

#include <cstdint>

namespace MOS6502 {
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
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), X register offset
        INS_LDA_ABS_X = 0xBD,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), Y register offset
        INS_LDA_ABS_Y = 0xB9,
        //cycles: 6  |    args: (8-bit zero-page address, X register offset)
        INS_LDA_IND_X = 0xA1,
        //cycles: 5, +1 if page crossed |    args: (8-bit zero-page address), Y register offset
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
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), Y register offset
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

        //cycles: 2  |    args: none (Implied)
        INS_TAX = 0xAA,
        //cycles: 2  |    args: none (Implied)
        INS_TAY = 0xA8,
        //cycles: 2  |    args: none (Implied)
        INS_TXA = 0x8A,
        //cycles: 2  |    args: none (Implied)
        INS_TYA = 0x98,

        //cycles: 2  |    args: none (Implied)
        INS_TXS = 0x9A,
        //cycles: 2  |    args: none (Implied)
        INS_TSX = 0xBA,
        //cycles: 3  |    args: none (Implied)
        INS_PHA = 0x48,
        //cycles: 3  |    args: none (Implied)
        INS_PHP = 0x08,
        //cycles: 4  |    args: none (Implied)
        INS_PLA = 0x68,
        //cycles: 4  |    args: none (Implied)
        INS_PLP = 0x28,

        //cycles: 2  |    args: 8-bit value
        INS_AND_IM = 0x29,
        //cycles: 3  |    args: 8-bit zero-page address
        INS_AND_ZP = 0x25,
        //cycles: 4  |    args: 8-bit zero-page address, X register offset
        INS_AND_ZP_X = 0x35,
        //cycles: 4  |    args: 16-bit absolute address (little endian)
        INS_AND_ABS = 0x2D,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), X register offset
        INS_AND_ABS_X = 0x3D,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), Y register offset
        INS_AND_ABS_Y = 0x39,
        //cycles: 6  |    args: (8-bit zero-page address, X register offset)
        INS_AND_IND_X = 0x21,
        //cycles: 5-6|    args: (8-bit zero-page address), Y register offset
        INS_AND_IND_Y = 0x31,

        //cycles: 2  |    args: 8-bit value
        INS_ORA_IM = 0x09,
        //cycles: 3  |    args: 8-bit zero-page address
        INS_ORA_ZP = 0x05,
        //cycles: 4  |    args: 8-bit zero-page address, X register offset
        INS_ORA_ZP_X = 0x15,
        //cycles: 4  |    args: 16-bit absolute address (little endian)
        INS_ORA_ABS = 0x0D,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), X register offset
        INS_ORA_ABS_X = 0x1D,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), Y register offset
        INS_ORA_ABS_Y = 0x19,
        //cycles: 6  |    args: (8-bit zero-page address, X register offset)
        INS_ORA_IND_X = 0x01,
        //cycles: 5, +1 if page crossed |    args: (8-bit zero-page address), Y register offset
        INS_ORA_IND_Y = 0x11,

        //cycles: 2  |    args: 8-bit value
        INS_EOR_IM = 0x49,
        //cycles: 3  |    args: 8-bit zero-page address
        INS_EOR_ZP = 0x45,
        //cycles: 4  |    args: 8-bit zero-page address, X register offset
        INS_EOR_ZP_X = 0x55,
        //cycles: 4  |    args: 16-bit absolute address (little endian)
        INS_EOR_ABS = 0x4D,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), X register offset
        INS_EOR_ABS_X = 0x5D,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), Y register offset
        INS_EOR_ABS_Y = 0x59,
        //cycles: 6  |    args: (8-bit zero-page address, X register offset)
        INS_EOR_IND_X = 0x41,
        //cycles: 5, +1 if page crossed |    args: (8-bit zero-page address), Y register offset
        INS_EOR_IND_Y = 0x51,

        //cycles: 3  |    args: 8-bit zero-page address
        INS_BIT_ZP = 0x24,
        //cycles: 4  |    args: 16-bit absolute address (little endian)
        INS_BIT_ABS = 0x2C,

        //cycles: 6  |    args: 16-bit absolute address (little endian)
        INS_JSR = 0x20,
        //cycles: 6  |    args: none (Implied)
        INS_RTS = 0x60,
        //cycles: 3  |    args: 16-bit absolute address (little endian)
        INS_JMP_ABS = 0x4C,
        //cycles: 5  |    args: (16-bit indirect address (little endian))
        INS_JMP_IND = 0x6C,

        //cycles: 2  |    args: none (Implied)
        INS_INX = 0xE8,
        //cycles: 2  |    args: none (Implied)
        INS_INY = 0xC8,
        //cycles: 2  |    args: none (Implied)
        INS_DEX = 0xCA,
        //cycles: 2  |    args: none (Implied)
        INS_DEY = 0x88,

        //cycles: 5  |    args: 8-bit zero-page address
        INS_INC_ZP = 0xE6,
        //cycles: 6  |    args: 8-bit zero-page address, X register offset
        INS_INC_ZP_X = 0xF6,
        //cycles: 6  |    args: 16-bit absolute address (little endian)
        INS_INC_ABS = 0xEE,
        //cycles: 7  |    args: 16-bit absolute address (little endian), X register offset
        INS_INC_ABS_X = 0xFE,

        //cycles: 5  |    args: 8-bit zero-page address
        INS_DEC_ZP = 0xC6,
        //cycles: 6  |    args: 8-bit zero-page address, X register offset
        INS_DEC_ZP_X = 0xD6,
        //cycles: 6  |    args: 16-bit absolute address (little endian)
        INS_DEC_ABS = 0xCE,
        //cycles: 7  |    args: 16-bit absolute address (little endian), X register offset
        INS_DEC_ABS_X = 0xDE,

        //cycles: 2, +1 if succeeds, +2 if new page  |    args: 8-bit branch offset
        INS_BEQ = 0xF0,
        //cycles: 2, +1 if succeeds, +2 if new page  |    args: 8-bit branch offset
        INS_BNE = 0xD0,
        //cycles: 2, +1 if succeeds, +2 if new page  |    args: 8-bit branch offset
        INS_BCC = 0x90,
        //cycles: 2, +1 if succeeds, +2 if new page  |    args: 8-bit branch offset
        INS_BCS = 0xB0,
        //cycles: 2, +1 if succeeds, +2 if new page  |    args: 8-bit branch offset
        INS_BMI = 0x30,
        //cycles: 2, +1 if succeeds, +2 if new page  |    args: 8-bit branch offset
        INS_BPL = 0x10,
        //cycles: 2, +1 if succeeds, +2 if new page  |    args: 8-bit branch offset
        INS_BVC = 0x50,
        //cycles: 2, +1 if succeeds, +2 if new page  |    args: 8-bit branch offset
        INS_BVS = 0x70,

        //cycles: 2  |    args: none (Implied)
        INS_CLC = 0x18,
        //cycles: 2  |    args: none (Implied)
        INS_SEC = 0x38,
        //cycles: 2  |    args: none (Implied)
        INS_CLD = 0xD8,
        //cycles: 2  |    args: none (Implied)
        INS_SED = 0xF8,
        //cycles: 2  |    args: none (Implied)
        INS_CLI = 0x58,
        //cycles: 2  |    args: none (Implied)
        INS_SEI = 0x78,
        //cycles: 2  |    args: none (Implied)
        INS_CLV = 0xB8,

        //cycles: 2  |    args: 8-bit value
        INS_ADC_IM = 0x69,
        //cycles: 3  |    args: 8-bit zero-page address
        INS_ADC_ZP = 0x65,
        //cycles: 4  |    args: 8-bit zero-page address, X register offset
        INS_ADC_ZP_X = 0x75,
        //cycles: 4  |    args: 16-bit absolute address (little endian)
        INS_ADC_ABS = 0x6D,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), X register offset
        INS_ADC_ABS_X = 0x7D,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), Y register offset
        INS_ADC_ABS_Y = 0x79,
        //cycles: 6  |    args: (8-bit zero-page address, X register offset)
        INS_ADC_IND_X = 0x61,
        //cycles: 5, +1 if page crossed |    args: (8-bit zero-page address), Y register offset
        INS_ADC_IND_Y = 0x71,

        //cycles: 2  |    args: 8-bit value
        INS_SBC_IM = 0xE9,
        //cycles: 3  |    args: 8-bit zero-page address
        INS_SBC_ZP = 0xE5,
        //cycles: 4  |    args: 8-bit zero-page address, X register offset
        INS_SBC_ZP_X = 0xF5,
        //cycles: 4  |    args: 16-bit absolute address (little endian)
        INS_SBC_ABS = 0xED,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), X register offset
        INS_SBC_ABS_X = 0xFD,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), Y register offset
        INS_SBC_ABS_Y = 0xF9,
        //cycles: 6  |    args: (8-bit zero-page address, X register offset)
        INS_SBC_IND_X = 0xE1,
        //cycles: 5, +1 if page crossed |    args: (8-bit zero-page address), Y register offset
        INS_SBC_IND_Y = 0xF1,

        //cycles: 2  |    args: 8-bit value
        INS_CMP_IM = 0xC9,
        //cycles: 3  |    args: 8-bit zero-page address
        INS_CMP_ZP = 0xC5,
        //cycles: 4  |    args: 8-bit zero-page address, X register offset
        INS_CMP_ZP_X = 0xD5,
        //cycles: 4  |    args: 16-bit absolute address (little endian)
        INS_CMP_ABS = 0xCD,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), X register offset
        INS_CMP_ABS_X = 0xDD,
        //cycles: 4, +1 if page crossed |    args: 16-bit absolute address (little endian), Y register offset
        INS_CMP_ABS_Y = 0xD9,
        //cycles: 6  |    args: (8-bit zero-page address, X register offset)
        INS_CMP_IND_X = 0xC1,
        //cycles: 5, +1 if page crossed |    args: (8-bit zero-page address), Y register offset
        INS_CMP_IND_Y = 0xD1,

        //cycles: 2  |    args: none (accumulator)
        INS_ASL_A = 0x0A,
        //cycles: 5  |    args: 8-bit zero-page address
        INS_ASL_ZP = 0x06,
        //cycles: 6  |    args: 8-bit zero-page address, X register offset
        INS_ASL_ZP_X = 0x16,
        //cycles: 6  |    args: 16-bit absolute address (little endian)
        INS_ASL_ABS = 0x0E,
        //cycles: 7  |    args: 16-bit absolute address (little endian), X register offset
        INS_ASL_ABS_X = 0x1E,

        //cycles: 2  |    args: none (accumulator)
        INS_LSR_A = 0x4A,
        //cycles: 5  |    args: 8-bit zero-page address
        INS_LSR_ZP = 0x46,
        //cycles: 6  |    args: 8-bit zero-page address, X register offset
        INS_LSR_ZP_X = 0x56,
        //cycles: 6  |    args: 16-bit absolute address (little endian)
        INS_LSR_ABS = 0x4E,
        //cycles: 7  |    args: 16-bit absolute address (little endian), X register offset
        INS_LSR_ABS_X = 0x5E,

        //cycles: 2  |    args: 8-bit value
        INS_CPX_IM = 0xE0,
        //cycles: 3  |    args: 8-bit zero-page address
        INS_CPX_ZP = 0xE4,
        //cycles: 4  |    args: 16-bit absolute address (little endian)
        INS_CPX_ABS = 0xEC,

        //cycles: 2  |    args: 8-bit value
        INS_CPY_IM = 0xC0,
        //cycles: 3  |    args: 8-bit zero-page address
        INS_CPY_ZP = 0xC4,
        //cycles: 4  |    args: 16-bit absolute address (little endian)
        INS_CPY_ABS = 0xCC,

        //cycles: 2  |    args: none (Implied)
        INS_NOP = 0xEA,
    };
}

#endif //INC_6502_PROJECT_INSTRUCTIONS_H
