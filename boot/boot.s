;--------------------------------------------------------
;
;           boot.s
:
;--------------------------------------------------------

;Multiboot magic number
MBOOT_HEADER_MAGIC  equ 0x1BADB002

;0号位表示所有的引导模块将按照页（4KB）边界对齐
MBOOT_PAGE_ALIGN    equ 1<<0

;1号位通过Multiboot信息结构的mem_*域包括可用内存的信息
;(告诉GRUB把内存空间的信息包含在Multiboot的信息结构中)
MBOOT_MEM_INFO  equ 1<<1
