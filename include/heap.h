#ifndef INCLUDE_HEAP_H
#define INCLUDE_HEAP_H

#include "types.h"

//堆起始地址
#define HEAP_START  0xE0000000

//内存块管理结构
typedef struct header{
    struct header* prev;        //指向前一个内存块
    struct header* next;        //指向后一个内存块
    uint32_t allocted : 1;      //该内存块是否被分配
    uint32_t length   :31;      //这个内存块的长度
}header_t;

//初始化堆
void init_heap();
//申请内存的函数
void* kmalloc(uint32_t len);
//内存释放函数
void kfree(void* p);
//测试函数
void test_heap();

#endif                          //INCLUDE_HEAP_H