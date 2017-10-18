C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = nasm

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include
# -m32 生成32位代码； -ggdb和-gstabs+添加相关的调试信息； -nostdinc 不包含C语言4标准库中的头文件
# -fno-buildin 要求gcc不使用自己的内建函数，除非显式声明   -fno-stack-protector 不使用栈保护等机制
LD_FLAGS = -T scripts/kernel.ld -m elf_i386 -nostdlib
# -T scripts/kernel.ld 使用自己编写的链接器脚本   -m elf_i386 生成i386平台下的ELF格式的可执行文件
ASM_FLAGS = -f elf -g -F stabs

all:$(S_OBJECTS) $(C_OBJECTS) link update_image

.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	@echo 编译汇编文件 $<...
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo 链接内核文件...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o cdf_kernel


.PHONY:update_image
update_image:
	sudo mount floppy.img /mnt/ChouDouFu
	sudo cp cdf_kernel /mnt/ChouDouFu/boot/kernel
	sleep 1
	sudo umount /mnt/ChouDouFu

.PHONY:umount_image
mount_image:
	sudo umount /mnt/kernel

.PHONY:qemu
qemu:
	qemu -fda floppy.img -boot a

.PHONY:bochs
bochs:
	bochs -f tools/bochsrc.txt

.PHONY:debug
debug:
	qemu -S -s -fda floppy.img -boot a &
	sleep 1
	cgdb -x scripts/gdbinit

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) cdf_kernel