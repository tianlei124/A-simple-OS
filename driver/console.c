#include "console.h"
#include "types.h"
#include "vmm.h"

//VGA显示缓冲的起点是0xB8000
static uint16_t *video_memory = (uint16_t *)(0xB8000 + PAGE_OFFSET);

//屏幕“光标”的坐标
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void move_cursor(void)
{
    //屏幕是80字节宽
    uint16_t cursorLocation = cursor_y * 80 + cursor_x;

    //在这里用到的两个内部寄存器的编号为14和15，分别表示光标位置的高8位和低8位

    outb(0x3D4,14);
    outb(0x3D5,cursorLocation >> 8);
    outb(0x3D4,15);
    outb(0x3D5,cursorLocation);
}

static void scroll()
{
    //attribute_byte 被构造出一个黑底白字的描述格式
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0f);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    //cursor_y = 25的时候就应该换行了
    if(cursor_y == 25)
    {
        //将所有行的显示数据复制到上一行，第一行消失
        int i;

        for(i = 0 * 80;i < 24 * 80;i++)
            video_memory[i] = video_memory[i+80];
        //最后一行的数据用空格填充
        for(i = 24*80;i < 25* 80;i++)
            video_memory[i] = blank;
        
        //现在cursor_y=24
        cursor_y = 24;
    }
}

void console_clear(void)
{
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0f);
    uint16_t blank = 0x20 | (attribute_byte << 8);
 
    int i;
    for(i = 0;i < 80 * 25;i++)
        video_memory[i] = blank;
    
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void console_putc_color(char c, real_color_t back, real_color_t fore)
{
    uint8_t back_color = (uint8_t) back;
    uint8_t fore_color = (uint8_t) fore;

    uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0f);
    uint16_t attribute = attribute_byte << 8;

    //0x08是退格健的ASCII码
    //0x09是tab健的ASCII码
    if(c == 0x08 && cursor_x)
        cursor_x--;
    else if(c == 0x09)
        cursor_x = (cursor_x + 8) & ~(8-1);
    else if(c == '\r')
        cursor_x = 0;
    else if(c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else if(c >= ' ')
    {
        video_memory[cursor_y*80 + cursor_x] = c | attribute;
        cursor_x++;
    }

    //80个字符一行，满80就换行
    if(cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y++;
    }

    //滚动屏幕显示
    scroll();

    //移动光标
    move_cursor();
}

void console_write(char *cstr)
{
    while(*cstr)
        console_putc_color(*cstr++,rc_black,rc_white);
}

void console_write_color(char *cstr, real_color_t back, real_color_t fore)
{
    while(*cstr)
        console_putc_color(*cstr++,back,fore);
}

void console_write_digit(int32_t xx, uint8_t base, real_color_t back, real_color_t fore)
{
    static char digits[] = "0123456789abcdef";
    char buf[16];
    int i,sign;
    uint32_t x;

    if(sign = (xx < 0))
        x = -xx;
    else
        x = xx;
    
    i = 0;

    do{
        buf[i++] = digits[x%base];
    }while((x/base)!=0);

    if(sign)
        buf[i++]= '-';
    
    while(--i >= 0)
        console_putc_color(buf[i],back,fore);
}