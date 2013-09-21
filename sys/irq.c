#include <sys/idt.h>
#include <sys/isr.h>
#include <stdio.h>
#include <sys/irq.h>

unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned int year;

uint32_t tick = 0,sec = 0;
void read_rtc();
enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};
 
 
int get_update_in_progress_flag() {
      outb(cmos_address, 0x0A);
      return (inb(cmos_data) & 0x80);
}
 
 
unsigned char get_RTC_register(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}

unsigned char keyboard_map[128] =
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
    0,   0,   0,   0,   0,   0,   0,   0, // 67
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
    0,	/* All other keys are undefined */ //88
    '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+'
 
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
   divisor = 59659;

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
    if (tick % 20 == 0)
    {
        read_rtc();
        sec++;
    }
    if (regs.interrupt_number >= 32)
    {
      //Send reset signal to slave.
      outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);
}

int shift_pressed=0;
// keyboard_handler
void irq_handler_1(registers_t regs)
{
  unsigned char scancode;
  unsigned char status;
  //printf("Interrupt No = %d\n",regs.interrupt_number);
  // Read keyboard status
  status = inb(0x64);
  scancode = status;
  /* Read from the keyboard's data buffer */
  scancode = inb(0x60);

  // If the top bit of the byte we read from the keyboard is set, that means that a key has just been released
  if (scancode & 0x80)
  {
    if(scancode == 0x2A)
      shift_pressed=0;
    // You can use this one to see if the user released the shift, alt, or control keys... 
  }
  else
  {
    if(scancode == 0x2A)
      shift_pressed=1;
    else
    {
      if(shift_pressed)
      {
        if( keyboard_map[scancode] >=97 && keyboard_map[scancode] <=122 )
          putchar(keyboard_map[scancode]-32);
        if( scancode >=2 && scancode <= 13)
          putchar(keyboard_map[scancode+88]);
        shift_pressed=0;
      } 
      else
        putchar(keyboard_map[scancode]);
    }
  }
  outb(0xA0, 0x20);
  outb(0x20, 0x20);

}

void read_rtc() {
  unsigned char last_second;
  unsigned char last_minute;
  unsigned char last_hour;
  unsigned char last_day;
  unsigned char last_month;
  unsigned char last_year;
  unsigned char registerB;
 
  // Note: This uses the "read registers until you get the same values twice in a row" technique
  //       to avoid getting dodgy/inconsistent values due to RTC updates
 
  while (get_update_in_progress_flag());                // Make sure an update isn't in progress
  second = get_RTC_register(0x00);
  minute = get_RTC_register(0x02);
  hour = get_RTC_register(0x04);
  day = get_RTC_register(0x07);
  month = get_RTC_register(0x08);
  year = get_RTC_register(0x09);
 
  do {
        last_second = second;
        last_minute = minute;
        last_hour = hour;
        last_day = day;
        last_month = month;
        last_year = year;
 
        while (get_update_in_progress_flag());           // Make sure an update isn't in progress
        second = get_RTC_register(0x00);
        minute = get_RTC_register(0x02);
        hour = get_RTC_register(0x04);
        day = get_RTC_register(0x07);
        month = get_RTC_register(0x08);
        year = get_RTC_register(0x09);
        }
   while( (last_second != second) || (last_minute != minute) || (last_hour != hour) ||
           (last_day != day) || (last_month != month) || (last_year != year)  );
 
  registerB = get_RTC_register(0x0B);
 
  // Convert BCD to binary values if necessary
 
  if (!(registerB & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
  }

  printtime(hour,minute,second);
  //printf("%d:%d:%d\n",hour,minute,second);
}
