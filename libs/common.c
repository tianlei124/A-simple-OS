#include "common.h"

/*
inline关键字，加上这个关键字之后编译器会尝试在该函数的调用点
直接进行代码展开，而不是传统的函数调用。
*/

/***************内存读写********************/

//向端口写一个字节
inline void outb(uint16_t port,uint8_t value)
{
    __asm__ __volatile__("outb %1, %0" : : "dN" (port), "a" (value));
}

//从端口读一个字节
uint8_t inb(uint16_t port)
{
    uint8_t ret;

    __asm__ __volatile__("inb %1, %0" : "=a" (ret): "dN" (port));
    return ret;
}

//从端口读一个字
uint16_t inw(uint16_t port)
{
    uint16_t ret;

    __asm__ __volatile__("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}