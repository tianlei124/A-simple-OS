; 定义两个构造中断处理函数的宏，一个有错误代码，一个没有

; 没有错误代码的中断
%macro ISR_NOERRORCODE  1
[GLOBAL isr%1]
isr%1:
    cli                                 ;首先关闭中断
    push 0                              ; push 无效的中断错误代码
    push %1                             ; push 中断号
    jmp isr_common_stub
%endmacro

;有错误代码的中断
%macro ISR_ERRORCODE    1
[GLOBAL isr%1]
isr%1:
    cli
    push %1
    jmp isr_common_stub
%endmacro

;定义中断处理函数

; 0~19
ISR_NOERRORCODE 0
ISR_NOERRORCODE 1
ISR_NOERRORCODE 2
ISR_NOERRORCODE 3
ISR_NOERRORCODE 4
ISR_NOERRORCODE 5
ISR_NOERRORCODE 6
ISR_NOERRORCODE 7
ISR_ERRORCODE   8
ISR_NOERRORCODE 9
ISR_ERRORCODE   10
ISR_ERRORCODE   11
ISR_ERRORCODE   12
ISR_ERRORCODE   13
ISR_ERRORCODE   14
ISR_NOERRORCODE 15
ISR_NOERRORCODE 16
ISR_ERRORCODE   17
ISR_NOERRORCODE 18
ISR_NOERRORCODE 19
; 20~31
ISR_NOERRORCODE 20
ISR_NOERRORCODE 21
ISR_NOERRORCODE 22
ISR_NOERRORCODE 23
ISR_NOERRORCODE 24
ISR_NOERRORCODE 25
ISR_NOERRORCODE 26
ISR_NOERRORCODE 27
ISR_NOERRORCODE 28
ISR_NOERRORCODE 29
ISR_NOERRORCODE 30
ISR_NOERRORCODE 31
; 32~255
ISR_NOERRORCODE 255

[GLOBAL isr_common_stub]
[EXTERN isr_handler]
;中断处理函数
isr_common_stub:
    pusha                                   ; pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov ax,ds
    push eax                                ;保存数据段描述符

    mov ax,0x10                             ;加载内核数据段描述符
    mov ds,ax
    mov es,ax,
    mov fs,ax
    mov gs,ax
    mov ss,ax

    push esp                                ;此时的 esp 寄存器的值等价于 pt_regs 结构提的指针
    call isr_handler                        ;在C语言代码中
    add esp,4                               ;清除压入的参数

    pop ebx                                 ;恢复原来的数据段描述符
    mov ds,bx
    mov es,bx
    mov fs,bx
    mov gs,bx
    mov ss,bx

    popa                                    ; Pops edi,esi,ebp,esp,ebx,edx,ecx,eax
    add esp,8                               ;清理栈中的 error code 和 ISR
    iret                                    ;中断返回
.end:

[GLOBAL idt_flush]
idt_flush:
    mov eax,[esp+4]                         ;参数存入 eax 寄存器
    lidt [eax]                              ;加载到 IDTR
    ret
.end:


; 构造中断请求的宏
%macro IRQ  2
[GLOBAL irq%1]
irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

IRQ 0,  32
IRQ 1,  33
IRQ 2,  34
IRQ 3,  35
IRQ 4,  36
IRQ 5,  37
IRQ 6,  38
IRQ 7,  39
IRQ 8,  40
IRQ 9,  41
IRQ 10,  42
IRQ 11,  43
IRQ 12,  44
IRQ 13,  45
IRQ 14,  46
IRQ 15,  47

[GLOBAL irq_common_stub]
[EXTERN irq_handler]
irq_common_stub:
    pusha

    mov ax,ds
    push eax

    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    push esp
    call irq_handler
    add esp,4

    pop ebx
    mov ds,bx
    mov es,bx
    mov fs,bx
    mov gs,bx
    mov ss,bx

    popa
    add esp,8
    iret
.end: