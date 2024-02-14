#include "byte_parser.h"

uint8_t addr_len_glob;
uint8_t arch_glob;
uint8_t bytes_glob;

std::string = opcode_x32_00();

//                      FIRST WILL FINISH COMPRESSED ONES
//====================================================================================
/* addr_len:
//  0 - 16-bit
//  1 - 32-bit
//
// arch:
//  0 - RV32
//  1 - RV64
//  2 - RV128
//
// bytes IN BIG_ENDIAN FORMAT !!!
*/
std::string pars_bytes(uint8_t addr_len, uint8_t arch, uint32_t bytes) {
    std::string res {"FAILED?"};
    //addr_len_glob = addr_len;
    //arch_glob = arch;
    //bytes_glob = bytes;
    if (addr_len) {
        //
    } else {
        if (bytes[0]) {         //OPCODE:
            if (bytes[1]) {     // 10
                //
            } else {            // 00
                opcode_x32_00();
            }
        } else {
            if (bytes[1]) {     // 11
                //
            } else {            // 01
                //
            }
        }
    }
}

opcode_x32_00() {
    //
}