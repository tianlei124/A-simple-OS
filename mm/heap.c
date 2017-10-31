#include "debug.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"

//申请内存块
static void alloc_chunk(uint32_t start, uint32_t len);

//释放内存块
static void free_chunk(header_t* chunk);

//切分内存块
static void split_chunk(header_t* chunk, uint32_t len);

//合并内存块
static void glue_chunk(header_t* chunk);

static uint32_t heap_max = HEAP_START;

//内存块管理指针
header_t* heap_first;

void init_heap()
{
    heap_first = 0;
}

void* kmalloc(uint32_t len)
{
    //一个内存块的总长度 = 可用的内存区域长度 + 管理指针的长度
    len += sizeof(header_t);

    header_t* cur_header = heap_first;
    header_t* prev_header = 0;

    while(cur_header)
    {
        //如果当前内存块没有被申请而且长度大于待申请的块
        if(cur_header->allocted == 0 && cur_header->length >= len)
        {
            //切出来一块长度为申请长度的内存块
            split_chunk(cur_header,len);
            cur_header->allocted = 1;
            //返回分配好的内存块的起始指针,需要减去管理头指针的长度
            return (void *)((uint32_t)cur_header + sizeof(header_t));
        }
        //否则就一点一点往后推移指针
        prev_header = cur_header;
        cur_header = cur_header->next;
    }

    uint32_t chunk_start;

    //如果这个函数是第一次执行，那就初始化堆区的起始位置
    if(prev_header)
        chunk_start = (uint32_t)prev_header + prev_header->length;
    else
    {
        chunk_start = HEAP_START;
        heap_first = (header_t *)chunk_start;
    }

    //检查是否需要申请内存页
    alloc_chunk(chunk_start,len);
    cur_header = (header_t*)chunk_start;
    cur_header->prev = prev_header;
    cur_header->next = 0;
    cur_header->allocted = 1;
    cur_header->length = len;

    if(prev_header)
        prev_header->next = cur_header;

    return (void*) (chunk_start + sizeof(header_t));
}

void kfree(void* p)
{
    //指针由内存区域起始位置回退到管理结构，并将 allocted 标志置0
    header_t* header = (header_t*)((uint32_t)p - sizeof(header_t));
    header->allocted = 0;

    //将这次释放的内存块和其余空闲的内存区域粘起来
    glue_chunk(header);
}

void alloc_chunk(uint32_t start, uint32_t len)
{
    //如果当前堆的位置已经到顶，则申请分配内存页
    while(start + len > heap_max)
    {
        uint32_t page = pmm_alloc_page();
        map(pgd_kern, heap_max, page, PAGE_PRESENT | PAGE_WRITE);
        heap_max += PAGE_SIZE;
    }
}

void free_chunk(header_t* chunk)
{
    if(chunk->prev == 0)
        heap_first = 0;
    else
        chunk->prev->next = 0;

    //空闲的内存大小超过一页的话，就释放掉
    while((heap_max - PAGE_SIZE) >= (uint32_t)chunk)
    {
        heap_max -= PAGE_SIZE;
        uint32_t page;
        get_mapping(pgd_kern, heap_max, &page);
        unmap(pgd_kern, heap_max);
        pmm_free_page(page);
    }
}

void split_chunk(header_t* chunk, uint32_t len)
{
    //切分内存块之前要保证切过以后剩余的内存块至少可以容纳一个内存管理头指针的大小
    if(chunk->length - len > sizeof(header_t))
    {
        header_t* newchunk = (header_t*)((uint32_t)chunk + chunk->length);
        newchunk->prev = chunk;
        newchunk->next = chunk->next;
        newchunk->allocted = 0;
        newchunk->length = chunk->length - len;

        chunk->next = newchunk;
        chunk->length = len;
    }
}

void glue_chunk(header_t* chunk)
{
    //如果该内存块前有链内存块且未被使用则拼合
    if(chunk->next && chunk->next->allocted == 0)
    {
        chunk->length = chunk->length + chunk->next->length;
        if(chunk->next->next)
            chunk->next->next->prev = chunk;
    }   chunk = chunk->prev;

    //如果该内存后面没有链接内存块，就把这块内存释放掉
    if(chunk->next == 0)
        free_chunk(chunk);
}

void test_heap()
{
    printk_color(rc_black, rc_magenta, "Test kmalloc() & kfree() now...\n\n");

    void* addr1 = kmalloc(50);
    printk("kamlloc   50 bytes in 0x%X\n",addr1);
    void* addr2 = kmalloc(500);
    printk("kmalloc   500 bytes in 0x%X\n",addr2);
    void* addr3 = kmalloc(5000);
    printk("kmalloc   500 bytes in 0x%X\n",addr3);
    void* addr4 = kmalloc(50000);
    printk("kmalloc   500 bytes in 0x%X\n",addr4);

    printk("free memory in 0x%X\n",addr1);
    kfree(addr1);
    printk("free memory in 0x%X\n",addr2);
    kfree(addr2);
    printk("free memory in 0x%X\n",addr3);
    kfree(addr3);
    printk("free memory in 0x%X\n",addr4);
    kfree(addr4);
}