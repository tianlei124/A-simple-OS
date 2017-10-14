#ifndef INCLUDE_ELF_H
#define INCLUDE_ELF_H

#include "types.h"
#include "multiboot.h"

#define ELF32_ST_TYPE(i)    ((i)&0xf)

//ELF可执行文件格式的各段声明

//section header
typedef struct elf_section_header_t{
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    uint32_t addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
}__attribute__((packed)) elf_section_header_t;

//symbol
typedef struct elf_symbol_t{
    uint32_t name;
    uint32_t value;
    uint32_t size;
    uint8_t info;
    uint8_t other;
    uint16_t shndx;
}__attribute__((packed)) elf_symbol_t;

//information
typedef struct elf_t{
    elf_symbol_t*   symtab;
    uint32_t        symtabsz;
    const char*      strtab;
    uint32_t        strtabsz;
}elf_t;

//从Multiboot结构体指针指向的内容中获取ELF信息
elf_t elf_from_multiboot(multiboot_t *mb);

//查看ELF的符号信息
const char* elf_loopup_symbol(uint32_t addr,elf_t *elf);

#endif