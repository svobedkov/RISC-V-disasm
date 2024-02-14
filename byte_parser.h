#include <map>
#include <string>

std::string reg_name(uint8_t num) {     //maybe uint16_t
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
    default: "ERROR";
    }
}

/* addr_len:            BAD IDEA, TO MANY CASES
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
std::string pars_bytes(int addr_len, uint8_t arch, uint32_t bytes);