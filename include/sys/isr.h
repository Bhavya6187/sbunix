#ifndef _ISR_H
#define _ISR_H

typedef struct registers
{
     uint64_t ds;                  // Data segment selector
     uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsp, rbp, rdi, rsi, rdx, rcx, rbx, rax; // Pushed by pushq i.e. all general purpose registers
     unsigned char interrupt_number, err_code;    // Interrupt number and error code (if applicable)
     //uint64_t rip, cs, eflags, usersp; // Pushed by the processor automatically.
} registers_t; 

#endif
