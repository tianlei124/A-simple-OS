#include "types.h"
#include "console.h"
#include "vargs.h"
#include "debug.h"


int kern_entry()
{
    init_debug();
    init_gdt();

    console_clear();
    printk_color(rc_black,rc_green,"This kernel'name is ChouDouFu!!!");

    return 0;
}