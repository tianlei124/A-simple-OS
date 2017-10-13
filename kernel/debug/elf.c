#include "common.h"
#include "elf.h"
#include "string.h"

//从Multiboot结构体指针指向的内容中获取ELF信息
elf_t elf_from_multiboot(multiboot_t *mb)
{
    int i;
    elf_t elf;
    elf_section_header_t *sh = (elf_section_header_t *)mb->addr;

    uint32_t shstrtab = sh[mb->shndx].addr;
    for(i = 0;i < mb->num;i++)
    {
        const char* name = (const char*) (shstrtab + sh[i].name);
        //在GRUB提供的multiboot信息中寻找ELF格式内核的字符串表和符号表
        if(strcmp(name,".strtab") == 0)
        {
            elf.strtab = (const char*) sh[i].addr;
            elf.strtabsz = sh[i].size;
        }
        if(strcmp(name,".symtab") == 0)
        {
            elf.symtab = (const char*) sh[i].addr;
            elf.symtabsz = (const char*) sh[i].size;
        }
    }
    return elf;
}

//查看ELF的符号信息
const char* elf_loopup_symbol(uint32_t addr,elf_t *elf)
{
    int i;

    for(i = 0;i < (elf->symtabsz / sizeof(elf_symbol_t));i++)
    {
        if(ELF32_ST_TYPE(elf->symtab[i].info)!=0x2)
            continue;
        //通过函数调用地址查找到函数的名字
        if((addr >= elf->symtab[i].value) && (addr < (elf->symtab[i].value + elf->symtab[i].size)))
            return (const char*) ((uint32_t) elf->strtab + elf->symtab[i].name);
    }
    return NULL;
}