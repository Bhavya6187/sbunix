#include <sys/idt.h>
#include <sys/isr.h>
#include <stdio.h>

#define PIC_EOI   0x20    /* End-of-interrupt command code */
#define PIC1      0x20
#define PIC2      0xA0
 
void PIC_sendEOI(unsigned char irq)
{
  if(irq >= 8)
    outb(PIC2,PIC_EOI);   
  outb(PIC1,PIC_EOI);
}

//Remapping PICs
void init_pic()
{
   outb(0x20, 0x11);
   outb(0xA0, 0x11);
   outb(0x21, 0x20); // pic 1 new 
   outb(0xA1, 0x28); // pic 2 new
   outb(0x21, 0x04);
   outb(0xA1, 0x02);
   outb(0x21, 0x01);
   outb(0xA1, 0x01);
   outb(0x21, 0x0);
   outb(0xA1, 0x0);
}

extern void  _irq_032();

void irq_handler_0(registers_t regs)
{
    printf("IRQ handler IRQ %d \n", regs.interrupt_number);
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.interrupt_number >= 40)
    {
      //Send reset signal to slave.
      outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);
    /* (interrupt_handlers[regs.int_no] != 0)
    {
      isr_t handler = interrupt_handlers[regs.int_no];
      handler(regs);
    }*/
}
