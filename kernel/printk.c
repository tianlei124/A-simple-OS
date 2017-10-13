#include "vargs.h"
#include "debug.h"

void printk(const char* format,...)
{
    int i;
    char c;
    va_list *args;

    va_start(args,format);
    for(i = 0;(c = format[i]&0xff) != 0;i++)
    {
        if(c != '%')
        {
            console_putc_color(c,rc_black,rc_white);
            continue;
        }

        c = format[++i]&0xff;
        if(c == 0)
            break;
        switch(c)
        {
            case 'd':
                console_write_digit(va_arg(args,int),10,1,rc_black,rc_white);
                break;
            case 'x':
                console_write_digit(va_arg(args,int),16,1,rc_black,rc_white);
            case 'p':
                console_write_digit(va_arg(args,int),16,1,rc_black,rc_white);
                break;
            case 's':
                console_write(va_arg(args,va_list));
                break;
            case '%':
                console_putc_color('%',rc_black,rc_white);
                break;
            default:
                //Print unknow % sequence to draw attention.
                console_putc_color('%',rc_black,rc_white);
                console_putc_color(c,rc_black,rc_white);
        }
    }
    va_end(args);
}

void printk_color(real_color_t back,real_color_t fore,const char* format,...)
{
    int i;
    char c;
    va_list *args;

    va_start(args,format);
    for(i = 0;(c = format[i]&0xff) != 0;i++)
    {
        if(c != '%')
        {
            console_putc_color(c,back,fore);
            continue;
        }

        c = format[++i]&0xff;
        if(c == 0)
            break;
        switch(c)
        {
            case 'd':
                console_write_digit(va_arg(args,int),10,1,back,fore);
                break;
            case 'x':

            case 'p':
                console_write_digit(va_arg(args,int),16,1,back,fore);
                break;
            case 's':
                console_putc_color(va_arg(args,va_list),back,fore);
                break;
            case '%':
                console_putc_color('%',back,fore);
                break;
            default:
                //Print unknow % sequence to draw attention.
                console_putc_color('%',back,fore);
                console_putc_color(c,back,fore);
        }
    }
    va_end(args);
}