#ifndef INCLUDE_MULTIBOOT_H
#define INCLUDE_MULTIBOOT_H

#include "types.h"

typedef struct multiboot_t{
    //变量声明顺序必须严格遵循 Multiboot 规范！！！！！！！ 就是以下顺序，不能变！！！ 不然会传错参数

    uint32_t flags;             //Multiboot 的版本信息
    /*
    *从 BIOS 获知的可用内存
    *
    *如果设置了 flags 中的第0位，则 mem_* 有效
    * mem_lower 和 mem_upper 分别指出了低端和高端内存的大小,单位是K
    *低端内存的首地址是0，高端内存的首地址是1M
    *低端内存的最大可能值为640K
    *高端内存的最大可能值是最大值减去1M，但是并不保证是这个值
    */
    uint32_t mem_lower;
    uint32_t mem_upper;

    /*
    *如果设置了 flags 中的第1位，则 boot_device有效。
    * boot_device 域由以下4个单字节的子域组成：
    *   +-------+-------+-------+-------+
    *   | drive | part1 | part2 | part3 |
    *   +-------+-------+-------+-------+
    *第一个字节包含驱动器号，它的格式与 BIOS 的 INTO0X13 低级磁盘接口相同： 例如，0x00
    * 代表第一个软盘驱动器， 0x80 代表第一个硬盘驱动器
    * 剩下的三个字节指出了引导分区， part1 指出顶级分区号， part2 指出一个顶级分区中的一个子分区... 
    */
    uint32_t boot_device;           //指出引导程序从哪个BIOS磁盘设备载入的OS映像

    uint32_t cmdline;               //内核命令行
    uint32_t mods_count;            //boot模块列表
    uint32_t mods_addr;

    /*
    * 如果设置了 flags 的第5位，则下面从 Mulitiboot 信息结构的第28位的域是有效的
    * addr 指出 ELF 格式内核映像section头表的位置， size指出 section头表中每一项的大小
    * num 指出一共有几项， shndx 指出作为名字索引的字符串表。
    */

    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;

    /*
    *以下两项指出保存由BIOS提供的内存分布的缓存区的地址和长度
    *mmap_addr 是缓冲区的地址，mmap_length 是缓冲区的总大小
    *缓冲区由一个或者多个下面的 mmap_entry_t 组成
    */

    uint32_t mmap_length;
    uint32_t mmap_addr;

    uint32_t drives_length;         //第一个驱动其结构的长度
    uint32_t drives_addr;           //第一个驱动器结构的物理地址
    uint32_t config_table;          //ROM配置表
    uint32_t bootloader_name;       //boot loader名
    uint32_t apm_table;             //APM表
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint32_t vbe_mode;
    uint32_t vbe_interface_seg;
    uint32_t vbe_interface_off;
    uint32_t vbe_interface_len;
} __attribute__((packed)) multiboot_t;

/*
* size 是相关结构的大小，单位是字节，它可能大于最小值 20
* base_addr_low 是启动地址的低32位，base_addr_high 是最高32位，启动地址总共有64位
* length_low 是内存区域大小的低32位，length_high 是内存区域大小的高 32位，总共是64位
* type 是相应地址区间的类型，1 代表可用RAM，所有其他值代表保留区域
*/

typedef struct mmap_entry_t {
    uint32_t size;
    uint32_t base_addr_low;
    uint32_t base_addr_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
} __attribute__((packed)) mmap_entry_t;

//声明一个全局的multiboot结构体指针
//内核未建立分页机制前暂存的指针
extern multiboot_t* mboot_ptr_tmp;
//内核页表建立之后的指针
extern multiboot_t* glb_mboot_ptr;

#endif                          //INCLUDE_MULTIBOOT_H