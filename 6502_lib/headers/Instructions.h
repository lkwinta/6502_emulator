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
        //cycles: 4-5|    args: 16-bit absolute address (little endian), X register offset
        INS_AND_ABS_X = 0x3D,
        //cycles: 4-5|    args: 16-bit absolute address (little endian), Y register offset
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
        //cycles: 4-5|    args: 16-bit absolute address (little endian), X register offset
        INS_ORA_ABS_X = 0x1D,
        //cycles: 4-5|    args: 16-bit absolute address (little endian), Y register offset
        INS_ORA_ABS_Y = 0x19,
        //cycles: 6  |    args: (8-bit zero-page address, X register offset)
        INS_ORA_IND_X = 0x01,
        //cycles: 5-6|    args: (8-bit zero-page address), Y register offset
        INS_ORA_IND_Y = 0x11,

        //cycles: 2  |    args: 8-bit value
        INS_EOR_IM = 0x49,
        //cycles: 3  |    args: 8-bit zero-page address
        INS_EOR_ZP = 0x45,
        //cycles: 4  |    args: 8-bit zero-page address, X register offset
        INS_EOR_ZP_X = 0x55,
        //cycles: 4  |    args: 16-bit absolute address (little endian)
        INS_EOR_ABS = 0x4D,
        //cycles: 4-5|    args: 16-bit absolute address (little endian), X register offset
        INS_EOR_ABS_X = 0x5D,
        //cycles: 4-5|    args: 16-bit absolute address (little endian), Y register offset
        INS_EOR_ABS_Y = 0x59,
        //cycles: 6  |    args: (8-bit zero-page address, X register offset)
        INS_EOR_IND_X = 0x41,
        //cycles: 5-6|    args: (8-bit zero-page address), Y register offset
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
    };
}

#endif //INC_6502_PROJECT_INSTRUCTIONS_H
