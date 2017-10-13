#ifndef INCLUDE_MULTIBOOT_h
#define INCLUDE_MULTIBOOT_h

#include "types.h"

typedef struct multiboot_t{
    uint32_t flags;             //Multiboot 的版本信息
    /*
    *从BIOS获知的可用内存
    *
    *mem_lower 和 mem_upper 分别指出了低端和高端内存的大小，单位时K
    *低端内存的首地址是0，高端内存的首地址是1M
    *低端内存的最大可能值为640K
    *高端内存的最大可能值是最大值减去1M，但是并不保证是这个值
    */

    /*
    *ELF格式可执行文件的section头表。包括每一項的大小、一共有几项以及作为名字索引的字符串
    */

    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;

    /*
    *以下两项指出保存由BIOS提供的内存分布的缓存区的地址和长度
    *mmap_addr 是缓冲区的地址，mmap_length 是缓冲区的地址
    *缓冲区由一个或者多个下面的mmap_entry_t组成
    */

    uint32_t mmap_addr;
    uint32_t mmap_length;

    uint32_t drives_length;         //第一个驱动其结构的长度
    uint32_t drives_addr;           //第一个驱动器结构的物理地址
    uint32_t config_table;          //ROM配置表
    uint32_t bootloader_name;       //boot loader名
    uint32_t apm_table;             //APM表
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint32_t vbe_interface_seg;
    uint32_t vbe_interface_off;
    uint32_t vbe_interface_len;
} __attribute__((packed)) multiboot_t;

/*
* size 是相关结构的大小，单位是字节，它可能大于最小值 20
* base_addr_low 是启动地址的低32位，base_addr_high 是最高32位，启动地址总共有64位
* length_low 是内存区域大小的低32位，length_high 是内存区域大小的高 32位，总共是64位
* type 是相应地址区间的类型，1 代表可用，所有其他值代表保留区域RAM
*/

typedef struct mmap_entry_t{
    uint32_t size;
    uint32_t base_addr_low;
    uint32_t base_addr_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
} __attribute__((packed)) mmap_entry_t;

//声明一个全局的multiboot结构体指针
extern multiboot_t* glb_mboot_ptr;

#endif