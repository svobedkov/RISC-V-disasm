#define EI_NIDENT 16

#include <iostream>
#include <fstream>
#include <map>
#include <string>

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

std::map <uint16_t, std::string> opcode_copmp_map = {
    {0b0, "ADDI4SPN"},
    {0b1, "--------"},  //CHECK FOR EXTENTION
};

std::map <uint32_t, std::string> opcode_map = {
    {},
};


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <elf_file>" << std::endl;
        return 1;
    }

    //std::ifstream elfFile(argv[1], std::ios::binary);
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

    /*std::cout << "==================================================================================================================" << std::endl;

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
        if (elfSHeader.sh_type == 6) {
            //
        }
        std::cout << "==================================================================================================================" << std::endl;
    }*/



    return 0;
}