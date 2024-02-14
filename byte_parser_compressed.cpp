#include "byte_parser.h"

std::string opcode_x32_00();

/*class opcode_00 {
    private:
    uint8_t rs1{0};
    uint8_t rs2{0};
    uint8_t rd{0};
    uint8_t uimm{0};
    uint16_t nzuimm{0};
    public:
    opcode_00(uint8_t rs1, uint8_t rs2, uint8_t rd, uint8_t uimm, uint16_t nzuimm) {

    }

}*/

uint8_t arch_glob;
uint8_t bytes_glob;

std::string pars_bytes_comp(uint8_t arch, uint32_t bytes) {
    std::string res {"FAILED?"};
    if (((bytes & 0b1) == 0b1) && ((bytes & 0b10) == 0b10)) {
        
    } else {
        //
    }
}

std::string opcode_x32_00() {
    //
}