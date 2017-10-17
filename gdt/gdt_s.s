[GLOBAL gdt_flush]

gdt_flush:
    mov eax,[esp+4]         ; 参数存入 eax 寄存器
    lgdt [eax]              ; 加载到 GDTR 

    mov ax,0x10             ; 加载我们的数据段描述符
    mov ds,ax               ; 更新所有可以更新的寄存器
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    jmp 0x08:.flush         ; 远跳转，0X80是代码段描述符
                            ; 远跳的目的是清空流水线并串行化处理器
.flush:
    ret