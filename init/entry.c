#include "types.h"
#include "console.h"
#include "vargs.h"
#include "debug.h"
#include "idt.h"

int kern_entry()
{
    init_debug();
    init_gdt();
    init_idt();

    console_clear();
    printk_color(rc_black,rc_green,"This kernel'name is ChouDouFu!!!\n");

    asm volatile ("int $0x4");
    asm volatile ("int $0x5");

    return 0;
}