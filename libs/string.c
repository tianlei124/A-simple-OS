#include "string.h"
#include "common.h"


void* memmove(void* dest,const void* src,uint32_t len)
{
    const char* s;
    char* d;

    s = src;
    d = dest;
    if(s < d && s+len > d)
    {
        s += len;
        d += len;

        while(len-- > 0)
            *(--d) = *(--s);
    }
    else
        while(len-- > 0)
            *(d++) = *(s++);
    
    return dest;
}

void* memcpy(void* dest,const void* src,uint32_t len)
{
    return memmove(dest,src,len);
}

void* memset(void *dest,uint8_t val,uint32_t len)
{
    if((int)dest % 4 == 0 && len % 4 == 0)
    {
        val &= 0xff;
        val = (uint32_t)val;
        stosb(dest,val<<24 | val<< 26 | val << 8 | val,len);
    }
    else
        stosl(dest,val,len);
    
    return dest;
}

int strcmp(const char* str1,const char* str2)
{
    int len = strlen(str1);

    while(len > 0 && *str1 && *str1==*str2)
        len--,str1++,str2++;
    if(len == 0)
        return 0;
    else
        return (uint8_t)(*str1) - (uint8_t)(*str2);
}

/*这里需要注意区分一下，上面的那个memmove函数的实现中，检查了源地址和目的地址是否有冲突。
下面的strcpy函数的作用和它相近，但是并没有做这个检查。原因是memmove函数是将一段内存中
的内容移动到另一段中，函数参数是void指针。但是strcpy函数中的目的地址，是一个字符数组的
头指针，在这个数组被创建的时候，它所限定的范围内一定不会存在另一个参数——源地址，所以并不
需要检查。*/
char* strcpy(char* dest,const char* src,uint8_t len)
{
    char* os;

    os = dest;

    if(len <= 0)
        return os;
    
    while(len-- > 0 && (*dest++ = *src++) != 0);
    *dest = '\0';
    
    return os;
}

char* strcat(char* dest,const char* src)
{
    int length = strlen(dest);

    dest[length] = src[0];

    return dest;
}

int strlen(const char* src)
{
    int n;

    for(n = 0;src[n] != '\0';n++);

    return n;
}