#ifndef _STDIO_H
#define _STDIO_H
#endif

//#include <unistd.h>
#define ROW 25
#define COLUMN 80
int position=0;
int scanf(const char *format, ...);
int printf(const char* str) {
	return 0;
}
static inline void outb( unsigned short port, unsigned char val )
{
      asm volatile( "outb %0, %1": : "a"(val), "Nd"(port) );
}
void update_cursor(int row, int col)
{
   int position=(row*80) + col;
   // cursor LOW port to vga INDEX register
   outb(0x3D4, 0x0F);
   outb(0x3D5, (unsigned char)(position&0xFF));
   // cursor HIGH port to vga INDEX register
   outb(0x3D4, 0x0E);
   outb(0x3D5, (unsigned char )((position>>8)&0xFF));
}

int putchar(char a )
{
  int color = 0x07;
	volatile char *video = (volatile char*)0xB8000;
  if(a=='\n')
  {
    position = ((position/80)+1)*80; 
    return 0;
  }
  *(video++ + 2*position) = a;
	*video++ = color;
  position++;
  return 0;
}

int puts(char* str)
{
  int strlength=0;
  while(*str)
  {
    putchar(*(str++));
    strlength++;
  }
  return strlength;
}

int clrscr()
{
  int i;
	int color = 0x07;
	volatile char *video = (volatile char*)0xB8000;
  for(i=0; i<ROW*COLUMN; i++)
  {
    *video++ = 0;
	  *video++ = color;
  }
  position=0;
  return 0;
}

int putint(int value)
{
  char * ptr=((void *)0);
  char * low=((void *)0);
  char *rc=((void *)0);
  rc = ptr;
  if ( value < 0 )
  {
    *ptr++ = '-';
  }
  low =ptr;
  do
  {
    // Modulo is negative for negative value. This trick makes abs() unnecessary.
    *ptr++ = "9876543210123456789"[9+ value%10];
    value /= 10;
  } while ( value );
  
  *ptr-- = '\0';
  while ( low < ptr )
  {
    char tmp = *low;
    *low++ = *ptr;
    *ptr-- = tmp;
  }
  puts(rc);
  return 0;
}

int int2hex(int value)
{
  char * ptr=((void *)0);
  char * low=((void *)0);
  char *rc=((void *)0);
  rc = ptr;
  low =ptr;
 
   
  do
  {
    int temp = value & 0x0f;
    if(temp < 10)
        *ptr++ = temp + '0';
     else
        *ptr++ = (temp) - 10 + 'A';
     value = value >> 4;

  } while ( value );
  
  *ptr++ = 'x';
  *ptr++ = '0';
  *ptr-- = '\0';
  while ( low < ptr )
  {
    char tmp = *low;
    *low++ = *ptr;
    *ptr-- = tmp;
  }
  while ( low < ptr )
  {
    char tmp = *low;
    *low++ = *ptr;
    *ptr-- = tmp;
  }
  puts(rc);
  return 0;
}
