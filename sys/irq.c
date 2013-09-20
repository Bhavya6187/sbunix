#include <sys/idt.h>
#include <sys/isr.h>
#include <stdio.h>
#include <sys/irq.h>

uint32_t tick = 0;

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};		

void PIC_sendEOI(unsigned char irq)
{
  if(irq >= 8)
    outb(PIC2,PIC_EOI);   
  outb(PIC1,PIC_EOI);
}

//Remapping PICs
void init_pic()
{
   printf("Remapping PIC... \n");
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
   uint16_t divisor;
//  divisor = (1193180/500);
   divisor = 65000;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   unsigned char l = (unsigned char)(divisor & 0xFF);
   unsigned char h = (unsigned char)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);

   printf("Initializing Timer... \n");
}

// timer_handler
void irq_handler_0(registers_t regs)
{
    tick++;
    //printf("IRQ handler IRQ %d  %d\n", regs.interrupt_number, tick);
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.interrupt_number >= 32)
    {
      //Send reset signal to slave.
      outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);
}

// keyboard_handler
void irq_handler_1(registers_t regs)
{
  unsigned char scancode;

  /* Read from the keyboard's data buffer */
  scancode = inb(0x60);

  /* If the top bit of the byte we read from the keyboard is
   * *     *  set, that means that a key has just been released */
  if (scancode & 0x80)
  {
    /* You can use this one to see if the user released the shift, alt, or control keys... */
  }
  else
  {
    putchar(kbdus[scancode]);
  }
}
