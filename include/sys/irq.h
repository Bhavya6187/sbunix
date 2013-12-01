#include "isr.h"
#define PIC_EOI   0x20    /* End-of-interrupt command code */
#define PIC1      0x20
#define PIC2      0xA0

void init_pic();
void irq_handler_0(registers_t regs);
void PIC_sendEOI(unsigned char irq);
void init_timer();
extern volatile unsigned char kbuf[128];
extern volatile int kbuf_index;
