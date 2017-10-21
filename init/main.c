#include "console.h"
#include "string.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"
#include "vmm.h"

// 开启分页机制之后的 Multiboot 数据指针
multiboot_t *glb_mboot_ptr;

// 开启分页机制之后的内核栈
char kern_stack[STACK_SIZE];

// 内核使用的临时页表和页目录
// 该地址必须是页对齐的地址，内存 0-640KB 肯定是空闲的
__attribute__((section(".init.data"))) pgd_t pgd_tmp[]  = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low  = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_hign = (pgd_t *)0x3000;

__attribute__((aligned(PAGE_SIZE))) pgd_tmp[PGD_SIZE] = {
	[0] = pte_low | PAGE_PRESENT | PAGE_WRITE,
	[PGD_INDEX(PAGE_OFFSET)] = pte_hign | PAGE_PRESENT | PAGE_WRITE,
};

/*
pgd_tmp[0] = (pgd_t)pte_low | PAGE_PRESENT | PAGE_WRITE;
pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (pgd_t)pte_hign | PAGE_PRESENT | PAGE_WRITE;
// 映射内核虚拟地址 4MB 到物理地址的前 4MB
int i;
for (i = 0; i < 1024; i++)
	pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;

// 映射 0x00000000-0x00400000 的物理地址到虚拟地址 0xC0000000-0xC0400000
for (i = 0; i < 1024; i++)
	pte_hign[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
*/

void kern_init()
{
	init_debug();
	init_gdt();
	init_idt();

	console_clear();
	printk_color(rc_black, rc_green, "Hello, OS kernel!\n\n");

	init_timer(200);

	// 开启中断
	// asm volatile ("sti");

	printk("kernel in memory start: 0x%08X\n", kern_start);
	printk("kernel in memory end:   0x%08X\n", kern_end);
	printk("kernel in memory used:   %d KB\n\n", (kern_end - kern_start) / 1024);
	
	show_memory_map();
	init_pmm();

	printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);

	uint32_t allc_addr = NULL;
	printk_color(rc_black, rc_light_brown, "Test Physical Memory Alloc :\n");
	allc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
	allc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
	allc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
	allc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);

	while (1)
		asm volatile ("hlt");
}