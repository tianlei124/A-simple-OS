#ifndef INCLUDE_IDT_H
#define INCLUDE_IDT_H

#include "types.h"

//初始化中断描述符表
void init_idt();

//中断描述符
typedef struct idt_entry_t{
    uint16_t base_low;              //中断处理函数地址 15～0 位
    uint16_t sel;                   //目标代码段描述符选择子
    uint8_t always0;                //置 0 段
    uint8_t flags;                  //一些标志位
    uint16_t base_high;             //中断处理函数地址 31～16 位
}__attribute__((packed)) idt_entry_t;

//IDTR
typedef struct idt_ptr_t{
    uint16_t limit;                 // 16位表界限
    uint32_t base;                  // 32位段基址
}__attribute__((packed)) idt_ptr_t;


//寄存器类型
typedef struct pt_regs_t{
    uint32_t ds;                    //保存用户的数据段描述符
    uint32_t edi;                   //从 edi 到 eax 由 pusha 指令压到栈中
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;                //中断号
    uint32_t err_code;               //错误代码
    uint32_t eip;                   //以下寄存器由处理器自动压栈
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
}pt_regs;

//中断处理函数指针
typedef void (*interrupt_handler_t) (pt_regs *);

//注册中断处理函数
void register_interrupt_handler(uint8_t n,interrupt_handler_t h);

//调用中断处理函数
void isr_handler(pt_regs *regs);

//中断号 0～19 属于 CPU 异常中断
//ISR 中断服务程序（interrupt service routine）
void isr0();                        //0 #DE 除0异常
void isr1();                        //1 #DB 调试异常
void isr2();                        //2 NMI
void isr3();                        //3 #BP 断点异常
void isr4();                        //4 #OF 溢出
void isr5();                        //5 #BR 对数组的引用越界
void isr6();                        //6 #UD 无效或未定义的操作吗
void isr7();                        //7 #NM 无数学协处理器（设备不可用）
void isr8();                        //8 #DF 双重故障 有错误代码
void isr9();                        //9 协处理器跨段操作
void isr10();                       //10 #TS 无效TSS 有错误代码
void isr11();                       //11 #NP 段不存在 有错误代码
void isr12();                       //12 #SS 栈错误 有错误代码
void isr13();                       //13 #GP 常规保护 有错误代码
void isr14();                       //14 #PF 页故障 有错误代码
void isr15();                       //15 CPU保留 
void isr16();                       //16 #MF 浮点处理单元错误
void isr17();                       //17 #AC 对齐检查
void isr18();                       //18 #MC 机器检查
void isr19();                       //19 #XM SIMD单指令多数据浮点异常

//20～31 Intel 保留
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();


//32~255 用户自定义
// 但是一般而言，32是 timer 中断，33是键盘中断
void isr255();

// IRQ 处理函数
void irq_handler(pt_regs* regs);

//声明 IRQ
#define IRQ0    32                  // timer
#define IRQ1    33                  // keyboard
#define IRQ2    34                  // 与 IRQ9 相接， MPU-401 MD 使用
#define IRQ3    35                  // Serial device
#define IRQ4    36                  // Serial device
#define IRQ5    37                  // 建议声卡使用
#define IRQ6    38                  // 软驱传输控制
#define IRQ7    39                  // 打印机传输控制
#define IRQ8    40                  // RealClock
#define IRQ9    41                  // 与 IRQ2 相接。可设定给其它硬件
#define IRQ10   42                  // 建议网卡使用
#define IRQ11   43                  // 建议 AGP 显卡使用
#define IRQ12   44                  // 接 PS/2 鼠标，也可以设置给其它硬件
#define IRQ13   45                  // 协处理器使用
#define IRQ14   46                  // IDE0 传输控制使用
#define IRQ15   47                  // IDE1 传输控制使用

//声明 IRQ 函数
// IRQ 中断请求
void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();


#endif                              //INCLUDE_IDT_H