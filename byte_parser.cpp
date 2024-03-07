#include "byte_parser.h"

uint8_t addr_len_glob;
uint8_t arch_glob;
uint8_t bytes_glob;

//std::string = opcode_x32_00();

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
