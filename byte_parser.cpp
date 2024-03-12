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

    // data[0] - rd
    // data[1] - rs1
    // data[2] - rs2
    // data[3] - rs3
    // data[4] - integer
void check_for_compressed_template(uint8_t *byte_array, uint32_t *data, uint8_t arch) {
    uint8_t func1;
    uint8_t func2;
    func1 = byte_array[0] & 0b00000011;
    func2 = byte_array[1] & 0b11100000 >> 5;
    switch (func1)
    {
    case 1:
        switch (func2)
        {
        case 0:
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) << 1);
            data[1] = data[0];
            data[4] = ((byte_array[0] & 0b01111100) >> 2) +
            ((byte_array[1] & 0b00010000) << 1);
            break;
        case 1:
            if (arch == 0) {
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
                data[4] = ((byte_array[0] & 0b01111100) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            }
            break;
        case 2:
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) << 1);
            data[4] = ((byte_array[0] & 0b01111100) >> 2) +
            ((byte_array[1] & 0b00010000) << 1);
            break;
        case 3:
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) << 1);
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
            data[2] = ((byte_array[1] & 0b00001100) >> 2);
            if (data[2] == 3) {
                data[1] = ((byte_array[0] & 0b00011100) >> 2);
                data[3] = ((byte_array[0] & 0b01100000) >> 5);
                data[4] = ((byte_array[1] & 0b00010000) >> 4);
            } else {
                data[4] = ((byte_array[0] & 0b01111100) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            }
            break;
        case 5:
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
            data[1] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00000011) << 1);
            data[4] = ((byte_array[0] & 0b00000100) << 3) +
            ((byte_array[0] & 0b00011000) >> 2) +
            ((byte_array[0] & 0b01100000) << 1) +
            ((byte_array[1] & 0b00001100) << 1) +
            ((byte_array[1] & 0b00010000) << 4);
            break;
        case 7:
            data[1] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00000011) << 1);
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
            data[4] = ((byte_array[0] & 0b01111100) >> 2) +
            ((byte_array[1] & 0b00010000) << 1);
            break;
        case 1:
            if (arch == 2) {
                data[0] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00001111) < 1);
                data[4] = ((byte_array[0] & 0b00111100) << 4) +
                ((byte_array[0] & 0b01000000) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            } else {
                data[0] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00001111) < 1);
                data[4] = ((byte_array[0] & 0b00011100) << 4) +
                ((byte_array[0] & 0b01100000) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            }
            break;
        case 2:
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) < 1);
            data[4] = ((byte_array[0] & 0b00001100) << 4) +
            ((byte_array[0] & 0b01110000) >> 2) +
            ((byte_array[1] & 0b00010000) << 1);
            break;
        case 3:
            if (arch == 0) {
                data[0] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00001111) < 1);
                data[4] = ((byte_array[0] & 0b00001100) << 4) +
                ((byte_array[0] & 0b01110000) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            } else {
                data[0] = ((byte_array[0] & 0b10000000) >> 7) +
                ((byte_array[1] & 0b00001111) < 1);
                data[4] = ((byte_array[0] & 0b00011100) << 4) +
                ((byte_array[0] & 0b01100000) >> 2) +
                ((byte_array[1] & 0b00010000) << 1);
            }
            break;
        case 4: // data[0] = [rd, rs1], data[1] = param1, data[2] = param2
            data[0] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00001111) < 1);
            data[1] = ((byte_array[0] & 0b01111100) >> 2);
            data[2] = ((byte_array[1] & 0b00010000) >> 4);
            break;
        case 5:
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
            data[4] = ((byte_array[0] & 0b10000000) >> 1) +
            ((byte_array[1] & 0b00000001) << 7) +
            ((byte_array[1] & 0b00011110) << 1);
            break;
        case 7:
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
            data[4] = ((byte_array[0] & 0b00100000) >> 2) + 
            ((byte_array[0] & 0b01000000) >> 4) +
            ((byte_array[0] & 0b10000000) >> 1) +
            ((byte_array[1] & 0b00000111) << 7) +
            ((byte_array[1] & 0b00011000) << 1);
            break;
        case 1:
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
            data[4] = ((byte_array[0] & 0b00100000) << 1) +
            ((byte_array[0] & 0b01000000) >> 4) +
            ((byte_array[1] & 0b00011100) << 1);
            break;
        case 3:
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
            data[2] = (byte_array[0] & 0b00011100) >> 2;
            data[1] = ((byte_array[0] & 0b10000000) >> 7) +
            ((byte_array[1] & 0b00000011) << 1);
            data[4] = ((byte_array[0] & 0b00100000) << 1) +
            ((byte_array[0] & 0b01000000) >> 4) +
            ((byte_array[1] & 0b00011100) << 1);
            break;
        case 7:
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

    // data[0] - rd
    // data[1] - rs1
    // data[2] - rs2
    // data[3] - rs3
    // data[4] - integer
void check_for_template(uint8_t *byte_array, uint32_t *data, uint8_t arch) {
    //
}