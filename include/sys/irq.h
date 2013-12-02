#include "isr.h"
#define PIC_EOI   0x20    /* End-of-interrupt command code */
#define PIC1      0x20
#define PIC2      0xA0

typedef struct regist
{
     uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rbp, rsi, rdx, rcx, rbx, rax; // Pushed by pushq i.e. all general purpose registers
     uint64_t rip, cs, eflags, rsp, ss; // Pushed by the processor automatically.
} registers_sch; 

extern registers_sch *regs_sch; 
void init_pic();
void irq_handler_0(registers_sch regs);
void PIC_sendEOI(unsigned char irq);
void init_timer();
extern volatile unsigned char kbuf[128];
extern volatile int kbuf_index;
