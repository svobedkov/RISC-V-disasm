#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

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

//
//
//

/*
int main() {
    TwoDimensionalMap<std::string, int, double> myMap;

    myMap.insert("Alice", 1, 3.14);
    myMap.insert("Bob", 2, 2.71);

    double value;
    if (myMap.find("Alice", 1, value)) {
        std::cout << "Found value: " << value << std::endl;
    } else {
        std::cout << "Value not found" << std::endl;
    }

    myMap.print();

    return 0;
}*/

class TemplateForBytes {
public:
    // 0 - for 16
    // 1 - for 32
    TemplateForBytes(uint8_t byte_len) {
        switch (byte_len)
        {
        case 1: //              15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
            // CIW-type           func3 |     nzuimm[5:4|9:6|2|3]  | rd  | op
            data_[0] = "???";
            break;
        default:
            break;
        }
    }


private:
    std::unordered_map<uint8_t, std::string> data_;
};



//template <typename KeyType1, typename KeyType2, typename ValueType>
class OpcodeAndFunction {
public:

    // 0 - for x32 (DEFAULT)
    // 1 - for x64
    // 2 - for x128
    OpcodeAndFunction(uint8_t arch) {
        //SPECIAL
        // 0, 4
        // 1, 4
        switch (arch)
        {
        case 1:
            data_[0][0][0] = "addi %s, sp, nzuimm[%d]";
            data_[0][1][0] = "lw %s, offset[%d](sp)";
            data_[0][2][0] = "lw %s, offset[%d](%s)";
            data_[0][3][0] = "ld %s, offset[%d](%s)";
            // 0, 4 RESERVED
            data_[0][5][0] = "fsd %s, offset[%d](%s)";
            data_[0][6][0] = "sw %s, offset[%d](%s)";
            data_[0][7][0] = "sd %s, offset[%d](%s)";
            data_[1][0][0] = "addi %s, %s, nzimm[%d]"; // if null, than (nop)
            data_[1][1][0] = "addiw %1$s, %1$s, %2$d"; // ??? (optimal?)
            data_[1][2][0] = "addi %s, zero, imm[%d]";
            data_[1][3][0] = "addi sp, sp, nzimm[%d]"; //==============================
            // 1, 4 COMPLEX STRUCTURE ???
            data_[1][5][0] = "jal zero, offset[%d]";
            data_[1][6][0] = "beq %s, zero, offset[%d]";
            data_[1][7][0] = "bne %s, zero, offset[%d]";
            data_[2][0][0] = "slli %1$s, %1$s, shamt[%2$d]";
            data_[2][1][0] = "fld %s, offset[%d](sp)";
            data_[2][2][0] = "lw %s, offset[%d](sp)";
            data_[2][3][0] = "ld %s, offset[%d](sp)";
            data_[2][4][0] = ""; // COMPLEX STRUCTURE
            data_[2][5][0] = "fsd %s, offset[%d](sp)";
            data_[2][6][0] = "sw %s, offset[%d](sp)";
            data_[2][7][0] = "sd %s, offset[%d](sp)";
            break;
        case 2:
            data_[0][0][0] = "addi %s, sp, nzuimm[%d]";
            data_[0][1][0] = "lq %s, offset[%d](%s)";
            data_[0][2][0] = "lw %s, offset[%d](%s)";
            data_[0][3][0] = "ld %s, offset[%d](%s)";
            // 0, 4 RESERVED
            data_[0][5][0] = "sq %s, offset[%d](%s)";
            data_[0][6][0] = "sw %s, offset[%d](%s)";
            data_[0][7][0] = "sd %s, offset[%d](%s)";
            data_[1][0][0] = "addi %s, %s, nzimm[%d]"; // if null, than (nop)
            data_[1][1][0] = "addiw %1$s, %1$s, %2$d"; // ??? (optimal?)
            data_[1][2][0] = "addi %s, zero, imm[%d]";
            data_[1][3][0] = "addi sp, sp, nzimm[%d]"; //==================================
            // 1, 4 COMPLEX STRUCTURE ???
            data_[1][5][0] = "jal zero, offset[%d]";
            data_[1][6][0] = "beq %s, zero, offset[%d]";
            data_[1][7][0] = "bne %s, zero, offset[%d]";
            data_[2][0][0] = "slli %1$s, %1$s, shamt[%2$d]"; // ADDITONAL
            data_[2][1][0] = "lq %s, offset[%d](sp)";
            data_[2][2][0] = "lw %s, offset[%d](sp)";
            data_[2][3][0] = "ld %s, offset[%d](sp)";
            data_[2][4][0] = ""; // COMPLEX STRUCTURE
            data_[2][5][0] = "sq %s, offset[%d](sp)";
            data_[2][6][0] = "sw %s, offset[%d](sp)";
            data_[2][7][0] = "sd %s, offset[%d](sp)";
            break;
        
        default:
            data_[0][0][0] = "addi %s, sp, nzuimm[%d]";
            data_[0][1][0] = "lw %s, offset[%d](sp)";
            data_[0][2][0] = "lw %s, offset[%d](%s)";
            data_[0][3][0] = "flw %s, offset[%d](%s)";
            // 0, 4 RESERVED
            data_[0][5][0] = "fsd %s, offset[%d](%s)";
            data_[0][6][0] = "sw %s, offset[%d](%s)";
            data_[0][7][0] = "fsw %s, offset[%d](%s)";
            data_[1][0][0] = "addi %s, %s, nzimm[%d]"; // if null, than (nop)
            data_[1][1][0] = "jal ra, offset[%d]";
            data_[1][2][0] = "addi %s, zero, imm[%d]";
            data_[1][3][0] = "addi sp, sp, nzimm[%d]"; //====================================
            // 1, 4 COMPLEX STRUCTURE ???  // data[0] = [rd, rs1], data[1] = rs2, data[2] = param1, data[3] = param2, (optional) data[4] = param3
            data_[1][5][0] = "jal zero, offset[%d]";
            data_[1][6][0] = "beq %s, zero, offset[%d]";
            data_[1][7][0] = "bne %s, zero, offset[%d]";
            data_[2][0][0] = "slli %1$s, %1$s, shamt[%2$d]";
            data_[2][1][0] = "fld %s, offset[%d](sp)";
            data_[2][2][0] = "lw %s, offset[%d](sp)";
            data_[2][3][0] = "flw %s, offset[%d](sp)";
            data_[2][4][0] = ""; // COMPLEX STRUCTURE
            data_[2][5][0] = "fsd %s, offset[%d](sp)";
            data_[2][6][0] = "sw %s, offset[%d](sp)";
            data_[2][7][0] = "fsw %s, offset[%d](sp)";
            break;
        }
        // RV32I Base Instruction Set
        data_[55][0][0] = "lui %s, %d"; //???
        data_[23][0][0] = "auipc %s, %d"; //???
        data_[111][0][0] = "jal %s, %d";
        data_[103][0][0] = "jalr %s, %s, %d";
        data_[99][0][0] = "beq %s, %s, %d";
        data_[99][1][0] = "bne %s, %s, %d";
        data_[99][4][0] = "blt %s, %s, %d";
        data_[99][5][0] = "bge %s, %s, %d";
        data_[99][6][0] = "bltu %s, %s, %d";
        data_[99][7][0] = "bgeu %s, %s, %d";
        data_[4][0][0] = "lb %s, %d(%s)";
        data_[4][1][0] = "lh %s, %d(%s)";
        data_[4][2][0] = "lw %s, %d(%s)";
        data_[4][4][0] = "lbu %s, %d(%s)";
        data_[4][5][0] = "lhu %s, %d(%s)";
        data_[35][0][0] = "sb %s, %d(%s)";
        data_[35][1][0] = "sh %s, %d(%s)";
        data_[35][2][0] = "sw %s, %d(%s)";
        data_[19][0][0] = "addi %s, %s, %d";
        data_[19][2][0] = "slti %s, %s, %d";
        data_[19][3][0] = "sltiu %s, %s, %d";
        data_[19][4][0] = "xori %s, %s, %d";
        data_[19][6][0] = "ori %s, %s, %d";
        data_[19][7][0] = "andi %s, %s, %d";
        data_[19][1][0] = "slli %s, %s, %d"; //
        data_[19][5][0] = "srli %s, %s, %d"; // EXPAND TO x64
        data_[19][5][32] = "srai %s, %s, %d"; //
        data_[51][0][0] = "add %s, %s, %s";
        data_[51][0][32] = "sub %s, %s, %s";
        data_[51][1][0] = "sll %s, %s, %s";
        data_[51][2][0] = "slt %s, %s, %s";
        data_[51][3][0] = "sltu %s, %s, %s";
        data_[51][4][0] = "xor %s, %s, %s";
        data_[51][5][0] = "srl %s, %s, %s";
        data_[51][5][32] = "sra %s, %s, %s";
        data_[51][6][0] = "or %s, %s, %s";
        data_[51][7][0] = "and %s, %s, %s";
        data_[15][0][0] = ""; //FENCE
        data_[15][1][0] = ""; //FENCE.I
        data_[115][0][0] = "ecall";
        data_[115][0][1] = "ebreak";
        data_[115][1][0] = "csrrw %s, %s, %s";
        data_[115][2][0] = "csrrs %s, %s, %s";
        data_[115][3][0] = "csrrc %s, %s, %s";
        data_[115][5][0] = "csrrwi %s, %s, %s";
        data_[115][6][0] = "csrrsi %s, %s, %s";
        data_[115][7][0] = "csrrci %s, %s, %s";
        // RV64I Base Instruction Set
        data_[3][4][0] = "lwu %s, %d(%s)"; // ???
        data_[3][3][0] = "ld %s, %d(%s)"; // ???
        data_[35][3][0] = "sd %s, %d(%s)";
        //data_[][][] = "";
        //data_[][][] = ""; SLL expanded to x64
        //data_[][][] = "";
        data_[27][0][0] = "addiw %s, %s, %d";
        data_[27][1][0] = "slliw %s, %s, %d";
        data_[27][5][0] = "srliw %s, %s, %d";
        data_[27][5][32] = "sraiw %s, %s, %d";
        data_[59][0][0] = "addw %s, %s, %s";
        data_[59][0][32] = "subw %s, %s, %s";
        data_[59][1][0] = "sllw %s, %s, %s";
        data_[59][5][0] = "srlw %s, %s, %s";
        data_[59][5][32] = "sraw %s, %s, %s";
        // RV32M Standard Extension
        data_[51][0][1] = "mul %s, %s, %s";
        data_[51][1][1] = "mulh %s, %s, %s";
        data_[51][2][1] = "mulhsu %s, %s, %s";
        data_[51][3][1] = "mulhu %s, %s, %s";
        data_[51][4][1] = "div %s, %s, %s";
        data_[51][5][1] = "divu %s, %s, %s";
        data_[51][6][1] = "rem %s, %s, %s";
        data_[51][7][1] = "remu %s, %s, %s";
        // RV64M Standard Extension
        data_[59][0][1] = "mulw %s, %s, %s";
        data_[59][4][1] = "divw %s, %s, %s";
        data_[59][5][1] = "divuw %s, %s, %s";
        data_[59][6][1] = "remw %s, %s, %s";
        data_[59][7][1] = "remuw %s, %s, %s";
        // RV32A Standard Extension
        data_[47][2][2] = "lr.w %s, (%s)";
        data_[47][2][3] = "sc.w %s, %s, (%s)";
        data_[47][2][1] = "amoswap.w %s, %s, (%s)";
        data_[47][2][0] = "amoadd.w %s, %s, (%s)";
        data_[47][2][4] = "amoxor.w %s, %s, (%s)";
        data_[47][2][12] = "amoand.w %s, %s, (%s)";
        data_[47][2][8] = "amoor.w %s, %s, (%s)";
        data_[47][2][16] = "amomin.w %s, %s, (%s)";
        data_[47][2][20] = "amomax.w %s, %s, (%s)";
        data_[47][2][24] = "amominu.w %s, %s, (%s)";
        data_[47][2][28] = "amomaxu.w %s, %s, (%s)";
        // RV64A Standard Extension
        data_[47][3][2] = "lr.d %s, (%s)";
        data_[47][3][3] = "sc.d %s, %s, (%s)";
        data_[47][3][1] = "amoswap.d %s, %s, (%s)";
        data_[47][3][0] = "amoadd.d %s, %s, (%s)";
        data_[47][3][4] = "amoxor.d %s, %s, (%s)";
        data_[47][3][12] = "amoand.d %s, %s, (%s)";
        data_[47][3][8] = "amoor.d %s, %s, (%s)";
        data_[47][3][16] = "amomin.d %s, %s, (%s)";
        data_[47][3][20] = "amomax.d %s, %s, (%s)";
        data_[47][3][24] = "amominu.d %s, %s, (%s)";
        data_[47][3][28] = "amomaxu.d %s, %s, (%s)";
        // RV32F Standard Extension UNNATURAL USE OF FUNC3
        data_[5][2][0] = "flw %s, offset[%d](%s)";
        data_[39][2][0] = "fsw %s, offset[%d](%s)";
        data_[67][0][0] = "fmadd.s %s, %s, %s, %s"; // UNNATURAL USE OF FUNC3
        data_[71][0][0] = "fmsub.s %s, %s, %s, %s"; // UNNATURAL USE OF FUNC3
        data_[75][0][0] = "fnmsub.s %s, %s, %s, %s"; // UNNATURAL USE OF FUNC3
        data_[79][0][0] = "fnmadd.s %s, %s, %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][0][0] = "fadd.s %s, %s, %s"; // NO FUNC 3
        data_[83][0][4] = "fsub.s %s, %s, %s"; // NO FUNC 3
        data_[83][0][8] = "fmul.s %s, %s, %s"; // NO FUNC 3
        data_[83][0][12] = "fdiv.s %s, %s, %s"; // NO FUNC 3
        data_[83][0][44] = "fsqrt.s %s, %s, %s"; // NO FUNC 3
        data_[83][0][16] = "fsgnj.s %s, %s, %s";
        data_[83][1][16] = "fsgnjn.s %s, %s, %s";
        data_[83][2][16] = "fsgnjx.s %s, %s, %s";
        data_[83][0][20] = "fmin.s %s, %s, %s";
        data_[83][1][20] = "fmax.s %s, %s, %s";
        data_[83][0][96] = "fcvt.w.s %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][1][96] = "fcvt.wu.s %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][0][112] = "fmv.x.w %s, %s";
        data_[83][2][80] = "feq.s %s, %s, %s";
        data_[83][1][80] = "flt.s %s, %s, %s";
        data_[83][0][80] = "fle.s %s, %s, %s";
        data_[83][1][112] = "fclass.s %s, %s, %s";
        data_[83][0][104] = "fcvt.s.w %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][1][104] = "fcvt.s.wu %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][0][120] = "fmv.w.x %s, %s";
        // RV64F Standard Extension
        data_[83][2][96] = "fcvt.l.s %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][3][96] = "fcvt.lu.s %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][2][104] = "fcvt.s.l %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][3][104] = "fcvt.s.lu %s, %s"; // UNNATURAL USE OF FUNC3
        // RV32D Standard Extension UNNATURAL USE OF FUNC3
        data_[5][3][0] = "fld %s, offset[%d](%s)";
        data_[39][3][0] = "fsd %s, offset[%d](%s)";
        data_[67][1][0] = "fmadd.d %s, %s, %s, %s"; // UNNATURAL USE OF FUNC3
        data_[71][1][0] = "fmsub.d %s, %s, %s, %s"; // UNNATURAL USE OF FUNC3
        data_[75][1][0] = "fnmsub.d %s, %s, %s, %s"; // UNNATURAL USE OF FUNC3
        data_[79][1][0] = "fnmadd.d %s, %s, %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][0][1] = "fadd.d %s, %s, %s"; // NO FUNC 3
        data_[83][0][5] = "fsub.d %s, %s, %s"; // NO FUNC 3
        data_[83][0][9] = "fmul.d %s, %s, %s"; // NO FUNC 3
        data_[83][0][13] = "fdiv.d %s, %s, %s"; // NO FUNC 3
        data_[83][0][45] = "fsqrt.d %s, %s, %s"; // NO FUNC 3
        data_[83][0][17] = "fsgnj.d %s, %s, %s";
        data_[83][1][17] = "fsgnjn.d %s, %s, %s";
        data_[83][2][17] = "fsgnjx.d %s, %s, %s";
        data_[83][0][21] = "fmin.d %s, %s, %s";
        data_[83][1][21] = "fmax.d %s, %s, %s";
        data_[83][1][32] = "fcvt.s.d %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][0][33] = "fcvt.d.s %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][2][81] = "feq.d %s, %s, %s";
        data_[83][1][81] = "flt.d %s, %s, %s";
        data_[83][0][81] = "fle.d %s, %s, %s";
        data_[83][1][113] = "fclass.d %s, %s, %s";
        data_[83][0][97] = "fcvt.w.d %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][1][97] = "fcvt.wu.d %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][0][105] = "fcvt.d.w %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][1][105] = "fcvt.d.wu %s, %s"; // UNNATURAL USE OF FUNC3
        // RV64D Standard Extension
        data_[83][2][97] = "fcvt.l.d %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][3][97] = "fcvt.lu.d %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][0][113] = "fmv.x.d %s, %s";
        data_[83][2][105] = "fcvt.d.l %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][3][105] = "fcvt.d.lu %s, %s"; // UNNATURAL USE OF FUNC3
        data_[83][0][121] = "fmv.d.x %s, %s";
    }

    void insert(const uint8_t& key1, const uint8_t& key2, const uint8_t& key3, const std::string& value) {
        data_[key1][key2][key3] = value;
    }

    bool find(const uint8_t& key1, const uint8_t& key2, const uint8_t& key3, std::string& value) const {
/*
        auto it1 = data_.find(key1);
        if (it1 != data_.end()) {
            auto it2 = it1->second.find(key2);
            if (it2 != it1->second.end()) {
                value = it2->second;
                return true;
            }
        }
        return false;
*/
        auto it1 = data_.find(key1);
        if (it1 != data_.end()) {
            auto it2 = it1->second.find(key2);
            if (it2 != it1->second.end()) {
                auto it3 = it2->second.find(key3);
                if (it3 != it2->second.end()) {
                    value = it3->second;
                    return true;
                }
            }
        }
        return false;

    }
private:
    std::unordered_map<uint8_t, std::unordered_map<uint8_t, std::unordered_map<uint8_t, std::string>>> data_;
};