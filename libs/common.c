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

/****************字符串处理********************/
//将一段内存地址初始化
static inline void
stosb(void* addr,uint8_t data,uint8_t cnt)
{
    __asm__ __volatile__("cld;rep stosb" :
                        "=D" (addr), "=c" (cnt) :
                        "0" (addr), "1" (cnt),"a" (data) :
                        "memory", "cc");
}
//和上一个函数的作用一样，代码也几乎一模一样，只是改变了一条汇编指令，把stosb改成了stosl
//stosl：Stores a Doubleword of data from EAX
static inline void
stosl(void* addr,uint8_t data,uint8_t cnt)
{
    __asm__ __volatile__("cld;rep stosl" :
                        "=D" (addr), "=c" (cnt) :
                        "0" (addr), "1" (cnt),"a" (data) :
                        "memory", "cc");
}