#ifndef _ISR_H
#define _ISR_H

typedef struct registers
{
     uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rdx, rcx, rbx, rax; // Pushed by pushq i.e. all general purpose registers
     unsigned char err_code;
     //unsigned char interrupt_number;    // Interrupt number and error code (if applicable)
     //uint64_t ds;                  // Data segment selector
     //uint64_t rip, cs, eflags, usersp; // Pushed by the processor automatically.
} registers_t; 

typedef struct myregs
{
     uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rdx, rcx, rbx, rax; // Pushed by pushq i.e. all general purpose registers
} myregs_t; 
#endif
