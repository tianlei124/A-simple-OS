#include "types.h"
#include "console.h"
#include "vargs.h"
#include "debug.h"


int kern_entry()
{
    int i = -100;

    init_debug();
    console_clear();
    printk("This OS kernel's name is ChouDouFu\ni = %d\n",i);
    panic("test");

    return 0;
}