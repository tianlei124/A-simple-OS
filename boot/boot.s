;--------------------------------------------------------
;
;           boot.s
;
;--------------------------------------------------------

;Multiboot magic number
MBOOT_HEADER_MAGIC  equ 0x1BADB002

;0号位表示所有的引导模块将按照页（4KB）边界对齐
MBOOT_PAGE_ALIGN    equ 1<<0

;1号位通过Multiboot信息结构的mem_*域包括可用内存的信息
;(告诉GRUB把内存空间的信息包含在Multiboot的信息结构中)
MBOOT_MEM_INFO      equ 1<<1

;定义Multiboot标记
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN|MBOOT_MEM_INFO

;域checksum是一个32位的无符号值，当与其他的magic域（也就是magic和flag）相加时,
;要求其结果必须是32位无符号值0（即magic+flags+checksum = 0）
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

;符合Multiboot规范的OS映像需要这样一个magic Multiboot头
;Multiboot头的分布必须如下表所示
;--------------------------------------------------------
; 偏移量   类型  域名      备注
;   0     u32   magic    必需
;   4     u32   flags    必需
;   8     u32   checksum 必需
;以上在这个程序中使用了的，更加详细的说明参见GNU相关文档
;--------------------------------------------------------

;--------------------------------------------------------

[BITS 32]          ;所有的代码以32bit的方式编译
section .text   ;
;在代码段的起始位置设置符合Mutliboot规范的标记

dd MBOOT_HEADER_MAGIC   ;GRUB会通过着一个magic number判断该映像是否支持Multiboot规范
dd MBOOT_HEADER_FLAGS   ;GRUB的一些加载时选项
dd MBOOT_CHECKSUM       
[GLOBAL start]          ;向外部声明内核代码入口，此处提供该声明给链接器
[GLOBAL glb_mboot_ptr]  ;向外部声明struct multiboot* 变量
[EXTERN kern_entry]     ;s声明内核C代码的入口函数

start:
    cli                     ;关闭硬件中断，防止CPU控制权因不知名的原因回到BIOS
    mov esp,STACK_TOP       ;设置内核栈头指针
    mov ebp,0               ;帧指针修改为0
    and esp,0FFFFFFF0H      ;栈地址按照16字节对齐
    mov [glb_mboot_ptr],ebx ;将ebx中存储的指针值放入全局变量
    call kern_entry         ;调用内核入口函数
stop:
    hlt                     ;停机指令，可以降低CPU的功耗
    jmp stop

;--------------------------------------------------------

section .bss
stack:
    resb 32768              ;这里作为内核栈
glb_mboot_ptr:              ;全局的Multiboot结构体指针
    resb 4

STACK_TOP equ $-stack-1

;--------------------------------------------------------