#include "types.h"
#include "console.h"
#include "vargs.h"
#include "debug.h"


int kern_entry()
{
    init_debug();
    console_clear();
    printk_color(rc_black,rc_green,"This OS kernel's name is ChouDouFu\n");
    panic("test");

    return 0;
}