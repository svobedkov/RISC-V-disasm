#include "byte_parser.h"
#include <sstream>
#include <iomanip>

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

    // data[0] - rd
    // data[1] - rs1
    // data[2] - rs2
    // data[3] - rs3
    // data[4] - integer

std::string reg_name(uint32_t num) {     //maybe uint16_t
    switch (num)
    {
    case 0: return "zero";
    case 1: return "ra";
    case 2: return "sp";
    case 3: return "gp";
    case 4: return "tp";
    case 5: return "t0";
    case 6: return "t1";
    case 7: return "t2";
    case 8: return "s0";
    case 9: return "s1";
    case 10: return "a0";
    case 11: return "a1";
    case 12: return "a2";
    case 13: return "a3";
    case 14: return "a4";
    case 15: return "a5";
    case 16: return "a6";
    case 17: return "a7";
    case 18: return "s2";
    case 19: return "s3";
    case 20: return "s4";
    case 21: return "s5";
    case 22: return "s6";
    case 23: return "s7";
    case 24: return "s8";
    case 25: return "s9";
    case 26: return "s10";
    case 27: return "s11";
    case 28: return "t3";
    case 29: return "t4";
    case 30: return "t5";
    case 31: return "t6";
    default: return "ERROR";
    }
}

std::string uint_to_hex( uint32_t i )
{
  std::stringstream stream;
  stream << "0x" 
//         << std::setfill ('0') << std::setw(sizeof(uint32_t)*2) 
         << std::hex << i;
  return stream.str();
}

std::string check_for_special_compressed_string(uint8_t func1, uint8_t func2, uint32_t* data, uint8_t arch) {
    switch (func1)
    {
        case 1:
            switch (func2)
            {
            case 0:
                if ((data[0] == 1) && (data[4] == 0)) {
                    return "nop";
                } else {
                    return "addi %1$s, %1$s, %5$x";
                }
                break;
            case 3:
                if (data[0] == 2) {
                    return "addi sp, sp, %5$x";
                } else {
                    return "lui %1$s, %5$x";
                }
                break;
            case 4:
                if (data[2] == 3) {
                    switch (data[3])
                    {
                    case 0:
                        if (data[4] == 1) {
                            return "subw %1$s, %1$s, %2$s";
                        } else {
                            return "sub %1$s, %1$s, %2$s";
                        }
                        break;
                    case 1:
                        if (data[4] == 1) {
                            return "addw %1$s, %1$s, %2$s";
                        } else {
                            return "xor %1$s, %1$s, %2$s";
                        }
                        break;
                    case 2:
                        return "or %1$s, %1$s, %2$s";
                        break;
                    case 3:
                        return "and %1$s, %1$s, %2$s";
                        break;                            
                    default:
                        break;
                    }
                } else {
                    switch (data[2])
                    {
                    case 0:
                        if (data[4] == 0) {
                            return "srli %1$s, %1$s, 64";
                        } else {
                            return "srli %1$s, %1$s, %5$x";
                        }
                        break;
                    case 1:
                        if (data[4] == 0) {
                            return "srai %1$s, %1$s, 64";
                        } else {
                            return "srai %1$s, %1$s, %5$x";
                        }
                        break;
                    case 2:
                        return "andi %1$s, %1$s, %5$x";
                        break;
                    default:
                        break;
                    }
                }
                break;                    
            default:
                break;
            }
            break;
        case 2:
            switch (func2)
            {
            case 0:
                if (data[4] == 0) {
                    return "slli %1$s, %1$s, 64";
                } else {
                    return "slli %1$s, %1$s, %5$x";
                }
                break;
            case 3:
                if (arch == 0) {
                    return "flw %1$s, %5$x(sp)";
                } else {
                    return "ld %1$s, %5$x(sp)";
                }
                break;
            case 4:
                switch (data[2])
                {
                case 0:
                    if (data[1] == 0) {
                        return "jalr zero, %1$s, 0";
                    } else {
                        return "add %1$s, zero, %2$s";
                    }
                    break;
                case 1:
                    if ((data[0] == 0) && (data[1] == 0)) {
                        return "ebreak";
                    } else if (data[1] == 0) {
                        return "jalr ra, %1$s, 0";
                    } else {
                        return "add %1$s, %1$s, %2$s";
                    }
                    break;                        
                default:
                    return "ERROR";
                    break;
                }
                break;
            default:
                return "ERROR";
                break;
            }
            break;                
        default:
            return "ERROR";
            break;
        }
    return "ERROR";
}

void check_for_compressed_template(uint8_t *byte_array, uint32_t *data, uint8_t arch) {
    uint8_t func1;
    uint8_t func2;
    func1 = (byte_array[0] & 0b00000011);
    func2 = (byte_array[1] & 0b11100000) >> 5;
    switch (func1)
    {
    case 1:
        switch (func2)
        {
        case 0:
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) << 1);
            data[1] = data[0];
            data[2] = 0;
            data[3] = 0;
            data[4] = ((byte_array[0] & 0b01111100) >> 2) +
            ((byte_array[1] & 0b00010000) << 1);
            break;
        case 1:
            if (arch == 0) {
                data[0] = 0;
                data[1] = 0;
                data[2] = 0;
                data[3] = 0;
                data[4] = ((byte_array[0] & 0b00000100) << 3) +
                ((byte_array[0] & 0b00111000) >> 2) +
                ((byte_array[0] & 0b01000000) << 1) +
                ((byte_array[0] & 0b10000000) >> 1) +
                ((byte_array[1] & 0b00000001) << 10) +
                ((byte_array[1] & 0b00000110) << 7) +
                ((byte_array[1] & 0b00001000) << 1) +
                ((byte_array[1] & 0b00010000) << 7);
            } else {
                data[0] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00001111) << 1);
                data[1] = data[0];
                data[2] = 0;
                data[3] = 0;
                data[4] = ((byte_array[0] & 0b01111100) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            }
            break;
        case 2:
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) << 1);
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            data[4] = ((byte_array[0] & 0b01111100) >> 2) +
            ((byte_array[1] & 0b00010000) << 1);
            break;
        case 3:
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) << 1);
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            if (data[0] == 2) {
                data[4] = ((byte_array[0] & 0b00000100) << 3) +
                ((byte_array[0] & 0b00011000) << 4) +
                ((byte_array[0] & 0b00100000) << 1) +
                ((byte_array[0] & 0b01000000) >> 2) +
                ((byte_array[1] & 0b00010000) << 5);
            } else {
                data[4] = ((byte_array[0] & 0b01111100) << 10) +
                ((byte_array[1] & 0b00010000) << 13);
            }
            break;
        case 4: // data[0] = [rd, rs1], data[1] = rs2, data[2] = param1, data[3] = param2, (optional) data[4] = param3
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00000011) << 1);
            data[1] = 0;
            data[2] = ((byte_array[1] & 0b00001100) >> 2);
            if (data[2] == 3) {
                data[1] = ((byte_array[0] & 0b00011100) >> 2);
                data[3] = ((byte_array[0] & 0b01100000) >> 5);
                data[4] = ((byte_array[1] & 0b00010000) >> 4);
            } else {
                data[3] = 0;
                data[4] = ((byte_array[0] & 0b01111100) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            }
            break;
        case 5:
            data[0] = 0;
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            data[4] = ((byte_array[0] & 0b00000100) << 3) +
            ((byte_array[0] & 0b00111000) >> 2) +
            ((byte_array[0] & 0b01000000) << 1) +
            ((byte_array[0] & 0b10000000) >> 1) +
            ((byte_array[1] & 0b00000001) << 10) +
            ((byte_array[1] & 0b00000110) << 7) +
            ((byte_array[1] & 0b00001000) << 1) +
            ((byte_array[1] & 0b00010000) << 7);
            break;
        case 6:
            data[0] = 0;
            data[1] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00000011) << 1);
            data[3] = 0;
            data[2] = 0;
            data[4] = ((byte_array[0] & 0b00000100) << 3) +
            ((byte_array[0] & 0b00011000) >> 2) +
            ((byte_array[0] & 0b01100000) << 1) +
            ((byte_array[1] & 0b00001100) << 1) +
            ((byte_array[1] & 0b00010000) << 4);
            break;
        case 7:
            data[0] = 0;
            data[1] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00000011) << 1);
            data[2] = 0;
            data[3] = 0;
            data[4] = ((byte_array[0] & 0b00000100) << 3) +
            ((byte_array[0] & 0b00011000) >> 2) +
            ((byte_array[0] & 0b01100000) << 1) +
            ((byte_array[1] & 0b00001100) << 1) +
            ((byte_array[1] & 0b00010000) << 4);
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (func2)
        {
        case 0:
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) << 1);
            data[1] = data[0];
            data[2] = 0;
            data[3] = 0;
            data[4] = ((byte_array[0] & 0b01111100) >> 2) +
            ((byte_array[1] & 0b00010000) << 1);
            break;
        case 1:
            if (arch == 2) {
                data[0] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00001111) << 1);
                data[1] = 0;
                data[2] = 0;
                data[3] = 0;
                data[4] = ((byte_array[0] & 0b00111100) << 4) +
                ((byte_array[0] & 0b01000000) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            } else {
                data[0] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00001111) << 1);
                data[1] = 0;
                data[2] = 0;
                data[3] = 0;
                data[4] = ((byte_array[0] & 0b00011100) << 4) +
                ((byte_array[0] & 0b01100000) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            }
            break;
        case 2:
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) << 1);
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            data[4] = ((byte_array[0] & 0b00001100) << 4) +
            ((byte_array[0] & 0b01110000) >> 2) +
            ((byte_array[1] & 0b00010000) << 1);
            break;
        case 3:
            if (arch == 0) {
                data[0] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00001111) << 1);
                data[1] = 0;
                data[2] = 0;
                data[3] = 0;
                data[4] = ((byte_array[0] & 0b00001100) << 4) +
                ((byte_array[0] & 0b01110000) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            } else {
                data[0] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00001111) << 1);
                data[1] = 0;
                data[2] = 0;
                data[3] = 0;
                data[4] = ((byte_array[0] & 0b00011100) << 4) +
                ((byte_array[0] & 0b01100000) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            }
            break;
        case 4: // data[0] = [rd, rs1], data[1] = param1, data[2] = param2
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) << 1);
            data[1] = ((byte_array[0] & 0b01111100) >> 2);
            data[2] = ((byte_array[1] & 0b00010000) >> 4);
            data[3] = 0;
            data[4] = 0;
            break;
        case 5:
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            if (arch == 2) {
                data[0] = ((byte_array[0] & 0b01111100) >> 2);
                data[4] = ((byte_array[0] & 0b10000000) >> 1) +
                ((byte_array[1] & 0b00000111) << 7) +
                ((byte_array[1] & 0b00011000) << 1);
            } else {
                data[0] = ((byte_array[0] & 0b01111100) >> 2);
                data[4] = ((byte_array[0] & 0b10000000) >> 1) +
                ((byte_array[1] & 0b00000011) << 7) +
                ((byte_array[1] & 0b00011100) << 1);
            }
            break;
        case 6:
            data[0] = ((byte_array[0] & 0b01111100) >> 2);
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            data[4] = ((byte_array[0] & 0b10000000) >> 1) +
            ((byte_array[1] & 0b00000001) << 7) +
            ((byte_array[1] & 0b00011110) << 1);
            break;
        case 7:
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            if (arch == 0) {
                data[0] = ((byte_array[0] & 0b01111100) >> 2);
                data[4] = ((byte_array[0] & 0b10000000) >> 1) +
                ((byte_array[1] & 0b00000001) << 7) +
                ((byte_array[1] & 0b00011110) << 1);
            } else {
                data[0] = ((byte_array[0] & 0b01111100) >> 2);
                data[4] = ((byte_array[0] & 0b10000000) >> 1) +
                ((byte_array[1] & 0b00000011) << 7) +
                ((byte_array[1] & 0b00011100) << 1);
            }
            break;
        default:
            break;
        }
        break;    
    case 0:
        switch (func2)
        {
        case 0:
            data[0] = (byte_array[0] & 0b00011100) >> 2;
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            data[4] = ((byte_array[0] & 0b00100000) >> 2) + 
            ((byte_array[0] & 0b01000000) >> 4) +
            ((byte_array[0] & 0b10000000) >> 1) +
            ((byte_array[1] & 0b00000111) << 7) +
            ((byte_array[1] & 0b00011000) << 1);
            break;
        case 1:
            data[2] = 0;
            data[3] = 0;
            if (arch == 2) {
                data[0] = (byte_array[0] & 0b00011100) >> 2;
                data[1] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00000011) << 1);
                data[4] = ((byte_array[0] & 0b01100000) << 1) +
                ((byte_array[1] & 0b00011000) << 1) +
                ((byte_array[1] & 0b00000100) << 6);
            } else {
                data[0] = (byte_array[0] & 0b00011100) >> 2;
                data[1] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00000011) << 1);
                data[4] = ((byte_array[0] & 0b01100000) << 1) +
                ((byte_array[1] & 0b00011100) << 1);
            }
            break;
        case 2:
            data[0] = (byte_array[0] & 0b00011100) >> 2;
            data[1] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00000011) << 1);
            data[2] = 0;
            data[3] = 0;
            data[4] = ((byte_array[0] & 0b00100000) << 1) +
            ((byte_array[0] & 0b01000000) >> 4) +
            ((byte_array[1] & 0b00011100) << 1);
            break;
        case 3:
            data[2] = 0;
            data[3] = 0;
            if (arch == 0) {
                data[0] = (byte_array[0] & 0b00011100) >> 2;
                data[1] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00000011) << 1);
                data[4] = ((byte_array[0] & 0b00100000) << 1) +
                ((byte_array[0] & 0b01000000) >> 4) +
                ((byte_array[1] & 0b00011100) << 1);
            } else {
                data[0] = (byte_array[0] & 0b00011100) >> 2;
                data[1] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00000011) << 1);
                data[4] = ((byte_array[0] & 0b01100000) << 1) +
                ((byte_array[1] & 0b00011100) << 1);
            }
            break;
        case 5:
            data[2] = 0;
            data[3] = 0;
            if (arch == 2) {
                data[0] = (byte_array[0] & 0b00011100) >> 2;
                data[1] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00000011) << 1);
                data[4] = ((byte_array[0] & 0b01100000) << 1) +
                ((byte_array[1] & 0b00011000) << 1) +
                ((byte_array[1] & 0b00000100) << 6);
            } else {
                data[0] = (byte_array[0] & 0b00011100) >> 2;
                data[1] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00000011) << 1);
                data[4] = ((byte_array[0] & 0b01100000) << 1) +
                ((byte_array[1] & 0b00011100) << 1);
            }
            break;
        case 6:
            data[0] = 0;
            data[3] = 0;
            data[2] = (byte_array[0] & 0b00011100) >> 2;
            data[1] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00000011) << 1);
            data[4] = ((byte_array[0] & 0b00100000) << 1) +
            ((byte_array[0] & 0b01000000) >> 4) +
            ((byte_array[1] & 0b00011100) << 1);
            break;
        case 7:
            data[0] = 0;
            data[3] = 0;
            if (arch == 0) {
                data[2] = (byte_array[0] & 0b00011100) >> 2;
                data[1] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00000011) << 1);
                data[4] = ((byte_array[0] & 0b00100000) << 1) +
                ((byte_array[0] & 0b01000000) >> 4) +
                ((byte_array[1] & 0b00011100) << 1);
            } else {
                data[2] = (byte_array[0] & 0b00011100) >> 2;
                data[1] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00000011) << 1);
                data[4] = ((byte_array[0] & 0b01100000) << 1) +
                ((byte_array[1] & 0b00011100) << 1);
            }
            break;     
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void R_type(uint8_t *byte_array, uint32_t *data) {  // FOR OPCODE 1010011 WE USE FLOAT/DOUBLE TEMPLATE data[3] - rm
    data[0] = ((byte_array[0] & 0b10000000) >> 7) +
    ((byte_array[1] & 0b00001111) << 1);
    data[1] = ((byte_array[1] & 0b10000000) >> 7) +
    ((byte_array[2] & 0b00001111) << 1);
    data[2] = ((byte_array[2] & 0b11110000) >> 4) +
    ((byte_array[3] & 0b00000001) << 4);
    data[3] = 0;
    data[4] = 0;
    data[5] = ((byte_array[1] & 0b01110000) >> 4);
    data[6] = ((byte_array[3] & 0b11111110) >> 1);
    if ((byte_array[0] & 0b01111111) == 0b1010011) {
        switch ((data[6] & 0b00110000) >> 4)
        {
        case 0:
            data[3] = data[5];
            data[5] = 0;
            break;
        case 2:
            data[3] = data[5];
            data[5] = data[2];
            data[2] = 0;
            break;
        default:
            break;
        }
    }
}

void R4_type(uint8_t *byte_array, uint32_t *data) { // data[4] - rm
    data[0] = ((byte_array[0] & 0b10000000) >> 7) +
    ((byte_array[1] & 0b00001111) << 1);
    data[1] = ((byte_array[1] & 0b10000000) >> 7) +
    ((byte_array[2] & 0b00001111) << 1);
    data[2] = ((byte_array[2] & 0b11110000) >> 4) +
    ((byte_array[3] & 0b00000001) << 4);
    data[3] = ((byte_array[3] & 0b11111000) >> 3);
    data[4] = ((byte_array[1] & 0b01110000) >> 4);
    data[6] = ((byte_array[3] & 0b00000110) >> 1);
}

void I_type(uint8_t *byte_array, uint32_t *data) {
    data[0] = ((byte_array[0] & 0b10000000) >> 7) +
    ((byte_array[1] & 0b00001111) << 1);
    data[1] = ((byte_array[1] & 0b10000000) >> 7) +
    ((byte_array[2] & 0b00001111) << 1);
    data[2] = 0;
    data[3] = 0;
    data[4] = ((byte_array[2] & 0b11110000) >> 4) +
    ((byte_array[3] & 0b11111111) << 4);
    data[5] = ((byte_array[1] & 0b01110000) >> 4);
    data[6] = 0;
    if (((byte_array[0] & 0b01111111) == 0b1110011) && data[5] == 0) {
        data[6] = data[4];
        data[4] = 0;
    }
    if (((byte_array[0] & 0b01111111) == 0b0010011) && ((data[5] == 0b001) || (data[5] == 0b101))) {
        data[6] = (data[4] & 0b111111100000) >> 5;
        data[4] = data[4] & 0b000000011111;
    }
}

void S_type(uint8_t *byte_array, uint32_t *data) {
    data[0] = 0;
    data[1] = ((byte_array[1] & 0b10000000) >> 7) +
    ((byte_array[2] & 0b00001111) << 1);
    data[2] = ((byte_array[2] & 0b11110000) >> 4) +
    ((byte_array[3] & 0b00000001) << 4);
    data[3] = 0;
    data[4] = ((byte_array[0] & 0b10000000) >> 7) +
    ((byte_array[1] & 0b00001111) << 1) +
    ((byte_array[3] & 0b11111110) << 4);
    data[5] = ((byte_array[1] & 0b01110000) >> 4);
    data[6] = 0;
}

void B_type(uint8_t *byte_array, uint32_t *data) {
    data[0] = 0;
    data[1] = ((byte_array[1] & 0b10000000) >> 7) +
    ((byte_array[2] & 0b00001111) << 1);
    data[2] = ((byte_array[2] & 0b11110000) >> 4) +
    ((byte_array[3] & 0b00000001) << 4);
    data[3] = 0;
    data[4] = ((byte_array[0] & 0b10000000) << 4) +
    ((byte_array[1] & 0b00001111) << 4) +
    ((byte_array[3] & 0b01111110) << 4) +
    ((byte_array[3] & 0b10000000) << 5);
    data[5] = ((byte_array[1] & 0b01110000) >> 4);
    data[6] = 0;
}

void U_type(uint8_t *byte_array, uint32_t *data) {
    data[0] = ((byte_array[0] & 0b10000000) >> 7) +
    ((byte_array[1] & 0b00001111) << 1);
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = ((byte_array[1] & 0b11110000) << 8) +
    ((byte_array[2] & 0b11111111) << 16) +
    ((byte_array[3] & 0b11111111) << 24);
    data[5] = 0;
    data[6] = 0;
}

void J_type(uint8_t *byte_array, uint32_t *data) {
    data[0] = ((byte_array[0] & 0b10000000) >> 7) +
    ((byte_array[1] & 0b00001111) << 1);
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = ((byte_array[1] & 0b11110000) << 8) +
    ((byte_array[2] & 0b00001111) << 16) +
    ((byte_array[2] & 0b00010000) << 7) +
    ((byte_array[2] & 0b11100000) >> 4) +
    ((byte_array[3] & 0b01111111) << 4) +
    ((byte_array[3] & 0b10000000) << 13);
    data[5] = 0;
    data[6] = 0;
}

void AMO_type(uint8_t *byte_array, uint32_t *data) { // data[3] - rl, data[4] - aq
    data[0] = ((byte_array[0] & 0b10000000) >> 7) +
    ((byte_array[1] & 0b00001111) << 1);
    data[1] = ((byte_array[1] & 0b10000000) >> 7) +
    ((byte_array[2] & 0b00001111) << 1);
    data[2] = ((byte_array[2] & 0b11110000) >> 4) +
    ((byte_array[3] & 0b00000001) << 4);
    data[3] = ((byte_array[3] & 0b00000010) >> 1);
    data[4] = ((byte_array[3] & 0b00000100) >> 2);
    data[5] = ((byte_array[1] & 0b01110000) >> 4);
    data[6] = ((byte_array[3] & 0b11111000) >> 3);
}

    // data[0] - rd
    // data[1] - rs1
    // data[2] - rs2
    // data[3] - rs3
    // data[4] - integer
    // data[5] - func1
    // data[6] - func2
void check_for_template(uint8_t *byte_array, uint32_t *data) {
    uint8_t opcode;
    opcode = byte_array[0] & 0b01111111;
    switch (opcode)
    {
    case 0b0110111:
        U_type(byte_array, data);
        break;
    case 0b0010111:
        U_type(byte_array, data);
        break;
    case 0b1101111:
        J_type(byte_array, data);
        break;
    case 0b1100111:
        I_type(byte_array, data);
        break;
    case 0b1100011:
        B_type(byte_array, data);
        break;
    case 0b0000011:
        I_type(byte_array, data);
        break;
    case 0b0100011:
        S_type(byte_array, data);
        break;
    case 0b0010011:
        I_type(byte_array, data);
        break;
    case 0b0110011:
        R_type(byte_array, data);
        break;
    case 0b0001111:
        I_type(byte_array, data);
        break;
    case 0b1110011:
        I_type(byte_array, data);
        break;
    case 0b0011011:
        R_type(byte_array, data);
        break;
    case 0b0111011:
        R_type(byte_array, data);
        break;
    case 0b0101111:
        AMO_type(byte_array, data);
        break;
    case 0b0000111:
        I_type(byte_array, data);
        break;
    case 0b0100111:
        S_type(byte_array, data);
        break;
    case 0b1000011:
        R4_type(byte_array, data);
        break;
    case 0b1000111:
        R4_type(byte_array, data);
        break;
    case 0b1001011:
        R4_type(byte_array, data);
        break;
    case 0b1001111:
        R4_type(byte_array, data);
        break;
    case 0b1010011:
        R_type(byte_array, data);
        break;
    default:
        break;
    }
}