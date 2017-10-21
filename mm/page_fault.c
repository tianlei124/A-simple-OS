#include "vmm.h"
#include "debug.h"

void page_fault(pt_regs* regs)
{
    uint32_t cr2;
    asm volatile("mov %%cr2, %0" : "=r" (cr2));

    printk("Page fault at 0x%x,virtual fualting address 0x%x\n",regs->eip,cr2);
    printk("Error code: %x\n",regs->err_code);

    // bit0 = 0 表示页面不在内存里
    if(!(regs->err_code & 0x1))
        printk_color(rc_black,rc_red,"Because the page wasn't present.\n");
    //bit1 = 0 表示读错误，等于1表示写错误
    if(regs->err_code & 0x2)
        printk_color(rc_black,rc_red,"Write error.\n");
    else
        printk_color(rc_black,rc_red,"Read error.\n");
    //bit2 = 1 表示中断发生在普通用户模式，等于0表示发生在超级用户模式
    if(regs->err_code & 0x4)
        printk_color(rc_black,rc_red,"The fault occurred in user mode.\n");
    else
        printk_color(rc_black,rc_red,"The fault occurred in supervisor mode.\n");
    //bit3 = 1 表示错误是由于保留位覆盖导致的
    if(regs->err_code & 0x8)
        printk_color(rc_black,rc_red,"Reserved bits being overwritten.\n");
    //bit4 = 1 表示错误发生在取指令的时候
    if(regs->err_code & 0x10)
        printk_color(rc_black,rc_red,"The fault occurred during an instruction fetch.\n");

    while(1);
}