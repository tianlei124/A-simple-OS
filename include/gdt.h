#ifndef ICLUDE_GDTH
#define ICLUDE_GDTH

#include "types.h"

//全局描述符类型
typedef struct gdt_entry_t {
    uint16_t limit_low;         //段界限 15～0
    uint16_t base_low;          //段基址 15～0
    uint8_t  base_middle;       //段基址 23～16
    uint8_t  access;            //段存在位、描述符特权级、描述符类型、描述符类别
    uint8_t  granularity;       //其他标志、段界限 19～16
    uint8_t  base_high;         //段基址 31～24
} __attribute__((packed)) gdt_entry_t;

// GDTR
typedef struct gdt_ptr_t {
    uint16_t limit;             //16位表界限
    uint32_t base;              //32位段基址,段基址是一个32位字
} __attribute__((packed)) gdt_ptr_t;

// 初始化全局描述符表
void init_gdt();
// GDT 加载到 GDTR 的函数
extern void gdt_flush(uint32_t);

#endif                          // ICLUDE_GDTH