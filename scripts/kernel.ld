/*
 *   kernel.ld -- 针对 kernel 格式所写的链接脚本。这个脚本告诉 ld 程序，如何构造内核映像文件。
 *   首先，声明内核程序的入口符号"start",然后声明第一个段 .text 段（代码段）以及它的起始地址 0x100000
 *   接下来是已初始化数据段 .data 和 未初始化数据段 .bss ,并且声明他们采用 4096 的页对齐方式
 *   LINUS GCC 增加了额外的数据段 .rodata ,这是一个只读的已初始化数据段，用来放置常量。简单起见，把它归在了 .data 段中
 */

ENTRY(start)
SECTIONS
{
	PROVIDE( kern_start = 0xC0100000);
	. = 0x100000; 
	.init.text : 
	{
		*(.init.text)
		. = ALIGN(4096);
	}
	.init.data : 
	{
		*(.init.data)
		. = ALIGN(4096);
	}

	. += 0xC0000000;
	.text : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text)
		. = ALIGN(4096);
	}
	.data : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}
	.bss : AT(ADDR(.bss) - 0xC0000000)
	{
		*(.bss)
		. = ALIGN(4096);
	}
	.stab : AT(ADDR(.stab) - 0xC0000000)
	{
		*(.stab)
		. = ALIGN(4096);
	}
	.stabstr : AT(ADDR(.stabstr) - 0xC0000000)
	{
		*(.stabstr)
	 	. = ALIGN(4096);
	}
	PROVIDE( kern_end = . );
	
	/DISCARD/ : { *(.comment) *(.eh_frame) }
}