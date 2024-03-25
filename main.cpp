#define EI_NIDENT 16

#include "byte_parser.h"
#include "intel_hex.h"

int elf_parser(char* argv[]);
int hex_parser(char* argv[]);

typedef struct {
        unsigned char   e_ident[EI_NIDENT];
        uint16_t    e_type;
        uint16_t    e_machine;
        uint32_t    e_version;
        uint32_t    e_entry;
        uint32_t    e_phoff;
        uint32_t    e_shoff;
        uint32_t    e_flags;
        uint16_t    e_ehsize;
        uint16_t    e_phentsize;
        uint16_t    e_phnum;
        uint16_t    e_shentsize;
        uint16_t    e_shnum;
        uint16_t    e_shstrndx;
} Elf32_Ehdr;

typedef struct {
        unsigned char   e_ident[EI_NIDENT];
        uint16_t    e_type;
        uint16_t    e_machine;
        uint32_t    e_version;
        uint64_t    e_entry;
        uint64_t    e_phoff;
        uint64_t    e_shoff;
        uint32_t    e_flags;
        uint16_t    e_ehsize;
        uint16_t    e_phentsize;
        uint16_t    e_phnum;
        uint16_t    e_shentsize;
        uint16_t    e_shnum;
        uint16_t    e_shstrndx;
} Elf64_Ehdr;

typedef struct {
        uint32_t    p_type;
        uint32_t    p_offset;
        uint32_t    p_vaddr;
        uint32_t    p_paddr;
        uint32_t    p_filesz;
        uint32_t    p_memsz;
        uint32_t    p_flags;
        uint32_t    p_align;
} Elf32_Phdr;

typedef struct {
        uint32_t    p_type;
        uint32_t    p_flags;
        uint64_t    p_offset;
        uint64_t    p_vaddr;
        uint64_t    p_paddr;
        uint64_t    p_filesz;
        uint64_t    p_memsz;
        uint64_t    p_align;
} Elf64_Phdr;

typedef struct {
        uint32_t    sh_name;
        uint32_t    sh_type;
        uint32_t    sh_flags;
        uint32_t    sh_addr;
        uint32_t    sh_offset;
        uint32_t    sh_size;
        uint32_t    sh_link;
        uint32_t    sh_info;
        uint32_t    sh_addralign;
        uint32_t    sh_entsize;
} Elf32_Shdr;

typedef struct {
        uint32_t    sh_name;
        uint32_t    sh_type;
        uint64_t    sh_flags;
        uint64_t    sh_addr;
        uint64_t    sh_offset;
        uint64_t    sh_size;
        uint32_t    sh_link;
        uint32_t    sh_info;
        uint64_t    sh_addralign;
        uint64_t    sh_entsize;
} Elf64_Shdr;

int main(int argc, char* argv[]) {
    if ((argc != 2) && (argv[1] != "-h")) {
        std::cerr << "Usage: " << argv[0] << " <elf_file>" << std::endl;
        std::cerr << "Alternative usage: " << argv[0] << " -h <hex_file> <x32/x64/x128>" << std::endl;
        return 1;
    }
}

int hex_parser(char* argv[]) {
    if ((argv[3] != "x32") && (argv[3] != "x64") && (argv[3] != "x128")) {
        std::cerr << "Unknown architecture" << std::endl;
        return 1;
    }

    std::string filename{argv[2]};
    std::vector<unsigned char> parsed_data = parse_intel_hex(filename);
    uint8_t temp_data[4];
    uint8_t counter = 0;
    uint8_t size_of_data = 0;

    for (unsigned char byte : parsed_data) {
        temp_data[counter] = byte;
        if (counter == 0) {
            switch (byte & 0b11)
            {
            case 3:
                size_of_data = 4;
                break;
            default:
                size_of_data = 2;
                break;
            }
        }
        counter++;
        if (counter == size_of_data) {
            counter = 0;
            switch (size_of_data)
            {
            case 2:
                /* code */
                break;
            case 4:
                /* code */
                break;            
            default:
                break;
            }
        }
    }
}

int elf_parser(char* argv[]) {
    std::ifstream elfFile(argv[1], std::ios::binary);
    if (!elfFile) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    // Сделать проверку на 32-битную/64-битную архитектуру(проверить бит в идентификаторе)
    Elf64_Ehdr elfHeader;
    elfFile.read(reinterpret_cast<char*>(&elfHeader), sizeof(Elf64_Ehdr));

    // Проверка на ELF магическое число
    if (elfHeader.e_ident[0] != 0x7F || elfHeader.e_ident[1] != 'E' || elfHeader.e_ident[2] != 'L' || elfHeader.e_ident[3] != 'F') {
        std::cerr << "Not an ELF file" << std::endl;
        return 1;
    }

    // Вывод информации о заголовке ELF

    std::cout << "ELF Header:" << std::endl;
    std::cout << "  Entry machine: " << std::hex << elfHeader.e_machine << std::endl;
    std::cout << "  Entry version: " << std::hex << elfHeader.e_version << std::endl;
    std::cout << "  Entry point address: 0x" << std::hex << elfHeader.e_entry << std::endl;
    std::cout << "  Entry phoff: " << std::hex << elfHeader.e_phoff << std::endl;
    std::cout << "  Entry shoff: " << std::hex << elfHeader.e_shoff << std::endl;
    std::cout << "  Flags: " << std::hex << elfHeader.e_flags << std::endl;
    std::cout << "  ELF header's size: 0x" << std::hex << elfHeader.e_ehsize << std::endl;
    std::cout << "  Size in bytes of one entry: " << std::hex << elfHeader.e_phentsize << std::endl;
    std::cout << "  Number of entries in the program header table: " << std::hex << elfHeader.e_phnum << std::endl;
    std::cout << "  Section header's size: " << std::hex << elfHeader.e_shentsize << std::endl;
    std::cout << "  Number of entries in the section header table: " << std::hex << elfHeader.e_shnum << std::endl;
    std::cout << "  Section header table index of the entry associated with the section name string table: " << std::hex << elfHeader.e_shstrndx << std::endl;

    std::cout << "==================================================================================================================" << std::endl;

int size_of_data;
// --------------------------------------------------------------------------------------------------
if (elfHeader.e_ident[4] == 1) {
    elfFile.seekg(elfHeader.e_phoff, std::ios_base::beg);
    for(int i = 0; i < elfHeader.e_phnum; i++) {
        Elf32_Phdr elfPHeader;
        elfFile.read(reinterpret_cast<char*>(&elfPHeader), sizeof(Elf32_Phdr));
        std::cout << "p_type: " << std::hex << elfPHeader.p_type << std::endl;
        std::cout << "p_flags: " << std::hex << elfPHeader.p_flags << std::endl;
        std::cout << "p_offset: " << std::hex << elfPHeader.p_offset << std::endl;
        std::cout << "p_vaddr: " << std::hex << elfPHeader.p_vaddr << std::endl;
        std::cout << "p_paddr: " << std::hex << elfPHeader.p_paddr << std::endl;
        std::cout << "p_filesz: " << std::hex << elfPHeader.p_filesz << std::endl;
        std::cout << "p_memsz: " << std::hex << elfPHeader.p_memsz << std::endl;
        std::cout << "p_align: " << std::hex << elfPHeader.p_align << std::endl;
        std::cout << "==================================================================================================================" << std::endl;
    }
    elfFile.seekg(elfHeader.e_shoff, std::ios_base::beg);
    for(int i = 0; i < elfHeader.e_shnum; i++) {
        Elf32_Shdr elfSHeader;
        elfFile.read(reinterpret_cast<char*>(&elfSHeader), sizeof(Elf32_Shdr));
        std::cout << "sh_name: " << std::hex << elfSHeader.sh_name << std::endl;
        std::cout << "sh_type: " << std::hex << elfSHeader.sh_type << std::endl;
        std::cout << "sh_flags: " << std::hex << elfSHeader.sh_flags << std::endl;
        std::cout << "sh_addr: " << std::hex << elfSHeader.sh_addr << std::endl;
        std::cout << "sh_offset: " << std::hex << elfSHeader.sh_offset << std::endl;
        std::cout << "sh_size: " << std::hex << elfSHeader.sh_size << std::endl;
        std::cout << "sh_link: " << std::hex << elfSHeader.sh_link << std::endl;
        std::cout << "sh_info: " << std::hex << elfSHeader.sh_info << std::endl;
        std::cout << "sh_addralign: " << std::hex << elfSHeader.sh_addralign << std::endl;
        std::cout << "sh_entsize: " << std::hex << elfSHeader.sh_entsize << std::endl;

        if (elfSHeader.sh_addr == elfHeader.e_entry) {
            size_of_data = elfSHeader.sh_size;
        }
        std::cout << "==================================================================================================================" << std::endl;
    }
} else {
    elfFile.seekg(elfHeader.e_phoff, std::ios_base::beg);
    for(int i = 0; i < elfHeader.e_phnum; i++) {
        Elf64_Phdr elfPHeader;
        elfFile.read(reinterpret_cast<char*>(&elfPHeader), sizeof(Elf64_Phdr));
        std::cout << "p_type: " << std::hex << elfPHeader.p_type << std::endl;
        std::cout << "p_flags: " << std::hex << elfPHeader.p_flags << std::endl;
        std::cout << "p_offset: " << std::hex << elfPHeader.p_offset << std::endl;
        std::cout << "p_vaddr: " << std::hex << elfPHeader.p_vaddr << std::endl;
        std::cout << "p_paddr: " << std::hex << elfPHeader.p_paddr << std::endl;
        std::cout << "p_filesz: " << std::hex << elfPHeader.p_filesz << std::endl;
        std::cout << "p_memsz: " << std::hex << elfPHeader.p_memsz << std::endl;
        std::cout << "p_align: " << std::hex << elfPHeader.p_align << std::endl;
        std::cout << "==================================================================================================================" << std::endl;
    }
    elfFile.seekg(elfHeader.e_shoff, std::ios_base::beg);
    for(int i = 0; i < elfHeader.e_shnum; i++) {
        Elf64_Shdr elfSHeader;
        elfFile.read(reinterpret_cast<char*>(&elfSHeader), sizeof(Elf64_Shdr));
        std::cout << "sh_name: " << std::hex << elfSHeader.sh_name << std::endl;
        std::cout << "sh_type: " << std::hex << elfSHeader.sh_type << std::endl;
        std::cout << "sh_flags: " << std::hex << elfSHeader.sh_flags << std::endl;
        std::cout << "sh_addr: " << std::hex << elfSHeader.sh_addr << std::endl;
        std::cout << "sh_offset: " << std::hex << elfSHeader.sh_offset << std::endl;
        std::cout << "sh_size: " << std::hex << elfSHeader.sh_size << std::endl;
        std::cout << "sh_link: " << std::hex << elfSHeader.sh_link << std::endl;
        std::cout << "sh_info: " << std::hex << elfSHeader.sh_info << std::endl;
        std::cout << "sh_addralign: " << std::hex << elfSHeader.sh_addralign << std::endl;
        std::cout << "sh_entsize: " << std::hex << elfSHeader.sh_entsize << std::endl;

        if (elfSHeader.sh_addr == elfHeader.e_entry) {
            size_of_data = elfSHeader.sh_size;
        }
        std::cout << "==================================================================================================================" << std::endl;
    }
}
//-------------------------------------------------------------------------------------------------------------------------
    // data[0] - rd
    // data[1] - rs1
    // data[2] - rs2
    // data[3] - rs3
    // data[4] - integer
    // FOR 32-bit COMMANDS
    // data[5] - func1
    // data[6] - func2
    OpcodeAndFunction strings_data(elfHeader.e_ident[4] - 1); // ARCH

    elfFile.seekg(elfHeader.e_entry, std::ios_base::beg);

    while (elfFile.tellg() < elfHeader.e_entry + size_of_data)
    {
        uint8_t temp_data[4];
        elfFile.read(reinterpret_cast<char*>(temp_data), sizeof(uint8_t));
        if ((temp_data[0] & 0b11) == 0b11) {
            uint32_t data[7];
            elfFile.read(reinterpret_cast<char*>(temp_data + 1), 3 * sizeof(uint8_t));
            check_for_template(temp_data, data);
            //
            std::string test_string;
            uint8_t opcode;
            opcode = temp_data[0] & 0b01111111;
            strings_data.find(opcode, data[5], data[6], test_string);
            //
            std::string parse_param;
            size_t start;
            parse_param = "%1$s";
            while ((start = test_string.find(parse_param)) != std::string::npos)
            {
                test_string.replace(start, 4, reg_name(data[0]));
            }
            parse_param = "%2$s";
            while ((start = test_string.find(parse_param)) != std::string::npos)
            {
                test_string.replace(start, 4, reg_name(data[1]));
            }
            parse_param = "%3$s";
            while ((start = test_string.find(parse_param)) != std::string::npos)
            {
                test_string.replace(start, 4, reg_name(data[2]));
            }
            parse_param = "%4$s";
            while ((start = test_string.find(parse_param)) != std::string::npos)
            {
                test_string.replace(start, 4, reg_name(data[3]));
            }
            parse_param = "%5$x";
            while ((start = test_string.find(parse_param)) != std::string::npos)
            {
                test_string.replace(start, 4, uint_to_hex(data[4]));
            }
            std::cout << test_string << std::endl;
            //
        } else {
            uint32_t data[5];
            elfFile.read(reinterpret_cast<char*>(temp_data + 1), sizeof(uint8_t));
            check_for_compressed_template(temp_data, data, elfHeader.e_ident[4] - 1); // ARCH
            //
            std::string test_string;
            uint8_t func1;
            uint8_t func2;
            func1 = (temp_data[0] & 0b00000011);
            func2 = (temp_data[1] & 0b11100000) >> 5;
            if ((1 <= func1) && (func1 <= 2) && (((3 <= func2) && (func2 <= 4)) || (func2 == 0))) {
                switch (func1)
                {
                case 1:
                    switch (func2)
                    {
                    case 0:
                        if ((data[0] == 1) && (data[4] == 0)) {
                            test_string = "nop";
                        } else {
                            test_string = "addi %1$s, %1$s, %5$x";
                        }
                        break;
                    case 3:
                        if (data[0] == 2) {
                            test_string = "addi sp, sp, %5$x";
                        } else {
                            test_string = "lui %1$s, %5$x";
                        }
                        break;
                    case 4:
                        if (data[2] == 3) {
                            switch (data[3])
                            {
                            case 0:
                                if (data[4] == 1) {
                                    test_string = "subw %1$s, %1$s, %2$s";
                                } else {
                                    test_string = "sub %1$s, %1$s, %2$s";
                                }
                                break;
                            case 1:
                                if (data[4] == 1) {
                                    test_string = "addw %1$s, %1$s, %2$s";
                                } else {
                                    test_string = "xor %1$s, %1$s, %2$s";
                                }
                                break;
                            case 2:
                                test_string = "or %1$s, %1$s, %2$s";
                                break;
                            case 3:
                                test_string = "and %1$s, %1$s, %2$s";
                                break;                            
                            default:
                                break;
                            }
                        } else {
                            switch (data[2])
                            {
                            case 0:
                                if (data[4] == 0) {
                                    test_string = "srli %1$s, %1$s, 64";
                                } else {
                                    test_string = "srli %1$s, %1$s, %5$x";
                                }
                                break;
                            case 1:
                                if (data[4] == 0) {
                                    test_string = "srai %1$s, %1$s, 64";
                                } else {
                                    test_string = "srai %1$s, %1$s, %5$x";
                                }
                                break;
                            case 2:
                                test_string = "andi %1$s, %1$s, %5$x";
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
                            test_string = "slli %1$s, %1$s, 64";
                        } else {
                            test_string = "slli %1$s, %1$s, %5$x";
                        }
                        break;
                    case 3:
                        if ((elfHeader.e_ident[4] - 1) == 0) {
                            test_string = "flw %1$s, %5$x(sp)";
                        } else {
                            test_string = "ld %1$s, %5$x(sp)";
                        }
                        break;
                    case 4:
                        switch (data[2])
                        {
                        case 0:
                            if (data[1] == 0) {
                                test_string = "jalr zero, %1$s, 0";
                            } else {
                                test_string = "add %1$s, zero, %2$s";
                            }
                            break;
                        case 1:
                            if ((data[0] == 0) && (data[1] == 0)) {
                                test_string = "ebreak";
                            } else if (data[1] == 0) {
                                test_string = "jalr ra, %1$s, 0";
                            } else {
                                test_string = "add %1$s, %1$s, %2$s";
                            }
                            break;                        
                        default:
                            break;
                        }
                        break;
                    default:
                        break;
                    }
                    break;                
                default:
                    break;
                }
            } else {
                strings_data.find(func1, func2, 0, test_string);
            }
            //
            std::string parse_param;
            size_t start;
            parse_param = "%1$s";
            while ((start = test_string.find(parse_param)) != std::string::npos)
            {
                test_string.replace(start, 4, reg_name(data[0]));
            }
            parse_param = "%2$s";
            while ((start = test_string.find(parse_param)) != std::string::npos)
            {
                test_string.replace(start, 4, reg_name(data[1]));
            }
            parse_param = "%3$s";
            while ((start = test_string.find(parse_param)) != std::string::npos)
            {
                test_string.replace(start, 4, reg_name(data[2]));
            }
            parse_param = "%4$s";
            while ((start = test_string.find(parse_param)) != std::string::npos)
            {
                test_string.replace(start, 4, reg_name(data[3]));
            }
            parse_param = "%5$x";
            while ((start = test_string.find(parse_param)) != std::string::npos)
            {
                test_string.replace(start, 4, uint_to_hex(data[4]));
            }
            std::cout << test_string << std::endl;
            //
        }
    }
    return 0;
}