#include <sys/idt.h>
#include <sys/isr.h>
#include <stdio.h>
#include <sys/irq.h>

uint32_t tick = 0; 
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

void init_timer()
{
   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor;
  divisor = (1193180/500);


   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   unsigned char l = (unsigned char)(divisor & 0xFF);
   unsigned char h = (unsigned char)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}

void irq_handler_0(registers_t regs)
{
    tick++;
    printf("IRQ handler IRQ %d  %d\n", regs.interrupt_number, tick);
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.interrupt_number >= 32)
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
