#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

extern std::string reg_name(uint32_t num);
void check_for_compressed_template(uint8_t *byte_array, uint32_t *data, uint8_t arch);
void check_for_template(uint8_t *byte_array, uint32_t *data);
std::string uint_to_hex(uint32_t i);

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
            data_[0][0][0] = "addi %1$s, sp, %5$x";
            data_[0][1][0] = "fld %1$s, %5$x(%2$s)";
            data_[0][2][0] = "lw %1$s, %5$x(%2$s)";
            data_[0][3][0] = "ld %1$s, %5$x(%2$s)";
            // 0, 4 RESERVED
            data_[0][5][0] = "fsd %2$s, %5$x(%1$s)";
            data_[0][6][0] = "sw %2$s, %5$x(%1$s)";
            data_[0][7][0] = "sd %2$s, %5$x(%1$s)";
            data_[1][0][0] = "addi %1$s, %1$s, %5$x"; // if null, than (nop)
            data_[1][1][0] = "addiw %1$s, %1$s, %5$x"; // ??? (optimal?)
            data_[1][2][0] = "addi %1$s, zero, %5$x";
            data_[1][3][0] = "addi sp, sp, nzimm[%d]"; //==============================
            // 1, 4 COMPLEX STRUCTURE ???
            data_[1][5][0] = "jal zero, %5$x";
            data_[1][6][0] = "beq %1$s, zero, %5$x";
            data_[1][7][0] = "bne %1$s, zero, %5$x";
            data_[2][0][0] = "slli %1$s, %1$s, shamt[%2$d]"; //===============
            data_[2][1][0] = "fld %1$s, %5$x(sp)";
            data_[2][2][0] = "lw %1$s, %5$x(sp)";
            data_[2][3][0] = "ld %1$s, %5$x(sp)"; // ==============
            data_[2][4][0] = ""; // COMPLEX STRUCTURE ===================
            data_[2][5][0] = "fsd %1$s, %5$x(sp)";
            data_[2][6][0] = "sw %1$s, %5$x(sp)";
            data_[2][7][0] = "sd %1$s, %5$x(sp)";
            break;
        case 2:
            data_[0][0][0] = "addi %1$s, sp, %5$x";
            data_[0][1][0] = "lq %1$s, %5$x(%2$s)";
            data_[0][2][0] = "lw %1$s, %5$x(%2$s)";
            data_[0][3][0] = "ld %1$s, %5$x(%2$s)";
            // 0, 4 RESERVED
            data_[0][5][0] = "sq %2$s, %5$x(%1$s)";
            data_[0][6][0] = "sw %2$s, %5$x(%1$s)";
            data_[0][7][0] = "sd %2$s, %5$x(%1$s)";
            data_[1][0][0] = "addi %1$s, %1$s, %5$x"; // if null, than (nop)
            data_[1][1][0] = "addiw %1$s, %1$s, %5$x"; // ??? (optimal?)
            data_[1][2][0] = "addi %1$s, zero, %5$x";
            data_[1][3][0] = "addi sp, sp, nzimm[%d]"; //==============================
            // 1, 4 COMPLEX STRUCTURE ???
            data_[1][5][0] = "jal zero, %5$x";
            data_[1][6][0] = "beq %1$s, zero, %5$x";
            data_[1][7][0] = "bne %1$s, zero, %5$x";
            data_[2][0][0] = "slli %1$s, %1$s, shamt[%2$d]";
            data_[2][1][0] = "lq %1$s, %5$x(sp)";
            data_[2][2][0] = "lw %1$s, %5$x(sp)";
            data_[2][3][0] = "ld %1$s, %5$x(sp)";
            data_[2][4][0] = ""; // COMPLEX STRUCTURE
            data_[2][5][0] = "sq %1$s, %5$x(sp)";
            data_[2][6][0] = "sw %1$s, %5$x(sp)";
            data_[2][7][0] = "sd %1$s, %5$x(sp)";
            break;
        
        default:
            data_[0][0][0] = "addi %1$s, sp, %5$x";
            data_[0][1][0] = "fld %1$s, %5$x(%2$s)";
            data_[0][2][0] = "lw %1$s, %5$x(%2$s)";
            data_[0][3][0] = "flw %1$s, %5$x(%2$s)";
            // 0, 4 RESERVED
            data_[0][5][0] = "fsd %2$s, %5$x(%1$s)";
            data_[0][6][0] = "sw %2$s, %5$x(%1$s)";
            data_[0][7][0] = "fsw %2$s, %5$x(%1$s)";
            data_[1][0][0] = "addi %1$s, %1$s, %5$x"; // if null, than (nop)
            data_[1][1][0] = "jal ra, %5$x"; // ??? (optimal?)
            data_[1][2][0] = "addi %1$s, zero, %5$x";
            data_[1][3][0] = "addi sp, sp, nzimm[%d]"; //==============================
            // 1, 4 COMPLEX STRUCTURE ???
            data_[1][5][0] = "jal zero, %5$x";
            data_[1][6][0] = "beq %1$s, zero, %5$x";
            data_[1][7][0] = "bne %1$s, zero, %5$x";
            data_[2][0][0] = "slli %1$s, %1$s, shamt[%2$d]";
            data_[2][1][0] = "fld %1$s, %5$x(sp)";
            data_[2][2][0] = "lw %1$s, %5$x(sp)";
            data_[2][3][0] = "flw %1$s, %5$x(sp)";
            data_[2][4][0] = ""; // COMPLEX STRUCTURE
            data_[2][5][0] = "fsd %1$s, %5$x(sp)";
            data_[2][6][0] = "sw %1$s, %5$x(sp)";
            data_[2][7][0] = "fsw %1$s, %5$x(sp)";
            break;
        }
        // RV32I Base Instruction Set
        data_[55][0][0] = "lui %1$s, %5$x"; //???
        data_[23][0][0] = "auipc %1$s, %5$x"; //???
        data_[111][0][0] = "jal %1$s, %5$x";
        data_[103][0][0] = "jalr %1$s, %2$s, %5$x";
        data_[99][0][0] = "beq %1$s, %2$s, %5$x";
        data_[99][1][0] = "bne %1$s, %2$s, %5$x";
        data_[99][4][0] = "blt %1$s, %2$s, %5$x";
        data_[99][5][0] = "bge %1$s, %2$s, %5$x";
        data_[99][6][0] = "bltu %1$s, %2$s, %5$x";
        data_[99][7][0] = "bgeu %1$s, %2$s, %5$x";
        data_[3][0][0] = "lb %2$s, %5$x(%1$s)";
        data_[3][1][0] = "lh %2$s, %5$x(%1$s)";
        data_[3][2][0] = "lw %2$s, %5$x(%1$s)";
        data_[3][4][0] = "lbu %2$s, %5$x(%1$s)";
        data_[3][5][0] = "lhu %2$s, %5$x(%1$s)";
        data_[35][0][0] = "sb %1$s, %5$x(%2$s)";
        data_[35][1][0] = "sh %1$s, %5$x(%2$s)";
        data_[35][2][0] = "sw %1$s, %5$x(%2$s)";
        data_[19][0][0] = "addi %1$s, %2$s, %5$x";
        data_[19][2][0] = "slti %1$s, %2$s, %5$x";
        data_[19][3][0] = "sltiu %1$s, %2$s, %5$x";
        data_[19][4][0] = "xori %1$s, %2$s, %5$x";
        data_[19][6][0] = "ori %1$s, %2$s, %5$x";
        data_[19][7][0] = "andi %1$s, %2$s, %5$x";
        data_[19][1][0] = "slli %1$s, %2$s, %5$x"; //
        data_[19][5][0] = "srli %1$s, %2$s, %5$x"; // EXPAND TO x64
        data_[19][5][32] = "srai %1$s, %2$s, %5$x"; //
        data_[51][0][0] = "add %1$s, %2$s, %3$s";
        data_[51][0][32] = "sub %1$s, %2$s, %3$s";
        data_[51][1][0] = "sll %1$s, %2$s, %3$s";
        data_[51][2][0] = "slt %1$s, %2$s, %3$s";
        data_[51][3][0] = "sltu %1$s, %2$s, %3$s";
        data_[51][4][0] = "xor %1$s, %2$s, %3$s";
        data_[51][5][0] = "srl %1$s, %2$s, %3$s";
        data_[51][5][32] = "sra %1$s, %2$s, %3$s";
        data_[51][6][0] = "or %1$s, %2$s, %3$s";
        data_[51][7][0] = "and %1$s, %2$s, %3$s";
        data_[15][0][0] = "fence"; //FENCE
        data_[15][1][0] = "fence.i"; //FENCE.I
        data_[115][0][0] = "ecall";
        data_[115][0][1] = "ebreak";
        data_[115][1][0] = "csrrw %1$s, %5$x, %2$s";
        data_[115][2][0] = "csrrs %1$s, %5$x, %2$s";
        data_[115][3][0] = "csrrc %1$s, %5$x, %2$s";
        data_[115][5][0] = "csrrwi %1$s, %5$x, %2$s";
        data_[115][6][0] = "csrrsi %1$s, %5$x, %2$s";
        data_[115][7][0] = "csrrci %1$s, %5$x, %2$s";
        // RV64I Base Instruction Set
        data_[3][6][0] = "lwu %2$s, %5$x(%1$s)"; // ???
        data_[3][3][0] = "ld %2$s, %5$x(%1$s)"; // ???
        data_[35][3][0] = "sd %1$s, %5$x(%2$s)";
        //data_[][][] = "";
        //data_[][][] = ""; SLL expanded to x64
        //data_[][][] = "";
        data_[27][0][0] = "addiw %1$s, %2$s, %5$x";
        data_[27][1][0] = "slliw %1$s, %2$s, %5$x";
        data_[27][5][0] = "srliw %1$s, %2$s, %5$x";
        data_[27][5][32] = "sraiw %1$s, %2$s, %5$x";
        data_[59][0][0] = "addw %1$s, %2$s, %3$s";
        data_[59][0][32] = "subw %1$s, %2$s, %3$s";
        data_[59][1][0] = "sllw %1$s, %2$s, %3$s";
        data_[59][5][0] = "srlw %1$s, %2$s, %3$s";
        data_[59][5][32] = "sraw %1$s, %2$s, %3$s";
        // RV32M Standard Extension
        data_[51][0][1] = "mul %1$s, %2$s, %3$s";
        data_[51][1][1] = "mulh %1$s, %2$s, %3$s";
        data_[51][2][1] = "mulhsu %1$s, %2$s, %3$s";
        data_[51][3][1] = "mulhu %1$s, %2$s, %3$s";
        data_[51][4][1] = "div %1$s, %2$s, %3$s";
        data_[51][5][1] = "divu %1$s, %2$s, %3$s";
        data_[51][6][1] = "rem %1$s, %2$s, %3$s";
        data_[51][7][1] = "remu %1$s, %2$s, %3$s";
        // RV64M Standard Extension
        data_[59][0][1] = "mulw %1$s, %2$s, %3$s";
        data_[59][4][1] = "divw %1$s, %2$s, %3$s";
        data_[59][5][1] = "divuw %1$s, %2$s, %3$s";
        data_[59][6][1] = "remw %1$s, %2$s, %3$s";
        data_[59][7][1] = "remuw %1$s, %2$s, %3$s";
        // RV32A Standard Extension
        data_[47][2][2] = "lr.w %2$s, %5$x(%1$s)";
        data_[47][2][3] = "sc.w %1$s, %5$x(%2$s)";
        data_[47][2][1] = "amoswap.w %1$s, %2$s, %3$s";
        data_[47][2][0] = "amoadd.w %1$s, %2$s, %3$s";
        data_[47][2][4] = "amoxor.w %1$s, %2$s, %3$s";
        data_[47][2][12] = "amoand.w %1$s, %2$s, %3$s";
        data_[47][2][8] = "amoor.w %1$s, %2$s, %3$s";
        data_[47][2][16] = "amomin.w %1$s, %2$s, %3$s";
        data_[47][2][20] = "amomax.w %1$s, %2$s, %3$s";
        data_[47][2][24] = "amominu.w %1$s, %2$s, %3$s";
        data_[47][2][28] = "amomaxu.w %1$s, %2$s, %3$s";
        // RV64A Standard Extension
        data_[47][3][2] = "lr.d %2$s, %5$x(%1$s)";
        data_[47][3][3] = "sc.d %1$s, %5$x(%2$s)";
        data_[47][3][1] = "amoswap.d %1$s, %2$s, %3$s";
        data_[47][3][0] = "amoadd.d %1$s, %2$s, %3$s";
        data_[47][3][4] = "amoxor.d %1$s, %2$s, %3$s";
        data_[47][3][12] = "amoand.d %1$s, %2$s, %3$s";
        data_[47][3][8] = "amoor.d %1$s, %2$s, %3$s";
        data_[47][3][16] = "amomin.d %1$s, %2$s, %3$s";
        data_[47][3][20] = "amomax.d %1$s, %2$s, %3$s";
        data_[47][3][24] = "amominu.d %1$s, %2$s, %3$s";
        data_[47][3][28] = "amomaxu.d %1$s, %2$s, %3$s";
        // RV32F Standard Extension UNNATURAL USE OF FUNC3
        data_[5][2][0] = "flw %1$s, %5$x(%2$s)";
        data_[39][2][0] = "fsw %1$s, %5$x(%2$s)";
        data_[67][0][0] = "fmadd.s %1$s, %2$s, %3$s, %4$s"; // UNNATURAL USE OF FUNC3
        data_[71][0][0] = "fmsub.s %1$s, %2$s, %3$s, %4$s"; // UNNATURAL USE OF FUNC3
        data_[75][0][0] = "fnmsub.s %1$s, %2$s, %3$s, %4$s"; // UNNATURAL USE OF FUNC3
        data_[79][0][0] = "fnmadd.s %1$s, %2$s, %3$s, %4$s"; // UNNATURAL USE OF FUNC3
        data_[83][0][0] = "fadd.s %1$s, %2$s, %3$s"; // NO FUNC 3
        data_[83][0][4] = "fsub.s %1$s, %2$s, %3$s"; // NO FUNC 3
        data_[83][0][8] = "fmul.s %1$s, %2$s, %3$s"; // NO FUNC 3
        data_[83][0][12] = "fdiv.s %1$s, %2$s, %3$s"; // NO FUNC 3
        data_[83][0][44] = "fsqrt.s %1$s, %2$s"; // NO FUNC 3
        data_[83][0][16] = "fsgnj.s %1$s, %2$s, %3$s";
        data_[83][1][16] = "fsgnjn.s %1$s, %2$s, %3$s";
        data_[83][2][16] = "fsgnjx.s %1$s, %2$s, %3$s";
        data_[83][0][20] = "fmin.s %1$s, %2$s, %3$s";
        data_[83][1][20] = "fmax.s %1$s, %2$s, %3$s";
        data_[83][0][96] = "fcvt.w.s %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][1][96] = "fcvt.wu.s %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][0][112] = "fmv.x.w %1$s, %2$s";
        data_[83][2][80] = "feq.s %1$s, %2$s, %3$s";
        data_[83][1][80] = "flt.s %1$s, %2$s, %3$s";
        data_[83][0][80] = "fle.s %1$s, %2$s, %3$s";
        data_[83][1][112] = "fclass.s %1$s, %2$s";
        data_[83][0][104] = "fcvt.s.w %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][1][104] = "fcvt.s.wu %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][0][120] = "fmv.w.x %1$s, %2$s";
        // RV64F Standard Extension
        data_[83][2][96] = "fcvt.l.s %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][3][96] = "fcvt.lu.s %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][2][104] = "fcvt.s.l %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][3][104] = "fcvt.s.lu %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        // RV32D Standard Extension UNNATURAL USE OF FUNC3
        data_[5][3][0] = "fld %1$s, %5$x(%2$s)";
        data_[39][3][0] = "fsd %1$s, %5$x(%2$s)";
        data_[67][1][0] = "fmadd.d %1$s, %2$s, %3$s, %4$s"; // UNNATURAL USE OF FUNC3
        data_[71][1][0] = "fmsub.d %1$s, %2$s, %3$s, %4$s"; // UNNATURAL USE OF FUNC3
        data_[75][1][0] = "fnmsub.d %1$s, %2$s, %3$s, %4$s"; // UNNATURAL USE OF FUNC3
        data_[79][1][0] = "fnmadd.d %1$s, %2$s, %3$s, %4$s"; // UNNATURAL USE OF FUNC3
        data_[83][0][1] = "fadd.d %1$s, %2$s, %3$s"; // NO FUNC 3
        data_[83][0][5] = "fsub.d %1$s, %2$s, %3$s"; // NO FUNC 3
        data_[83][0][9] = "fmul.d %1$s, %2$s, %3$s"; // NO FUNC 3
        data_[83][0][13] = "fdiv.d %1$s, %2$s, %3$s"; // NO FUNC 3
        data_[83][0][45] = "fsqrt.d %1$s, %2$s"; // NO FUNC 3
        data_[83][0][17] = "fsgnj.d %1$s, %2$s, %3$s";
        data_[83][1][17] = "fsgnjn.d %1$s, %2$s, %3$s";
        data_[83][2][17] = "fsgnjx.d %1$s, %2$s, %3$s";
        data_[83][0][21] = "fmin.d %1$s, %2$s, %3$s";
        data_[83][1][21] = "fmax.d %1$s, %2$s, %3$s";
        data_[83][1][32] = "fcvt.s.d %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][0][33] = "fcvt.d.s %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][2][81] = "feq.d %1$s, %2$s, %3$s";
        data_[83][1][81] = "flt.d %1$s, %2$s, %3$s";
        data_[83][0][81] = "fle.d %1$s, %2$s, %3$s";
        data_[83][1][113] = "fclass.d %1$s, %2$s";
        data_[83][0][97] = "fcvt.w.d %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][1][97] = "fcvt.wu.d %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][0][105] = "fcvt.d.w %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][1][105] = "fcvt.d.wu %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        // RV64D Standard Extension
        data_[83][2][97] = "fcvt.l.d %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][3][97] = "fcvt.lu.d %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][0][113] = "fmv.x.d %1$s, %2$s";
        data_[83][2][105] = "fcvt.d.l %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][3][105] = "fcvt.d.lu %1$s, %2$s"; // UNNATURAL USE OF FUNC3
        data_[83][0][121] = "fmv.d.x %1$s, %2$s";
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

