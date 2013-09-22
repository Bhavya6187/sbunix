#ifndef _STDIO_H
#define _STDIO_H
#endif

#include <stdarg.h>
//#include <unistd.h>
#define ROW 25
#define COLUMN 80
int position=0;
int scanf(const char *format, ...);

inline void outb( unsigned short port, unsigned char val )
{
   asm volatile( "outb %0, %1": : "a"(val), "Nd"(port) );
}

inline unsigned char inb( unsigned short port )
{
    unsigned char ret;
    asm volatile( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
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
  update_cursor(0,0);
  return 0;
}

int scrollup(int dist)
{
  int i;
  //int j;
	int color = 0x07;
	volatile char *video = (volatile char*)0xB8000;
  //volatile char *temp = (volatile char*)(video + dist*COLUMN);
  for(i=0; i<(ROW-dist)*COLUMN; i++)
  {
     *(video) = *(video + dist*COLUMN*2);
      video++; 
     *video++ = color;
  }
  
  for(i=COLUMN*(ROW-dist); i<ROW*COLUMN; i++)
  {
    *video++ = 0;
	  *video++ = color;
  }
  position=COLUMN*(ROW-dist);
  update_cursor((ROW-dist), COLUMN);
  return 0;
}

int putchar(char a )
{
  int color = 0x07;
	volatile char *video = (volatile char*)0xB8000;
  if(position>=ROW*COLUMN)
    scrollup(1);
  if(a=='\n')
  {
    position = ((position/80)+1)*80; 
    update_cursor(position/COLUMN, position%COLUMN);
    return 0;
  }
  *(video++ + 2*position) = a;
	*video++ = color;
  position++;
  update_cursor(position/COLUMN, position%COLUMN);
  return 1;
}

int puts(char* str)
{
  int strlength=0;
  while(*str)
  {
    putchar(*(str++));
    strlength++;
  }
  update_cursor(position/COLUMN, position%COLUMN);
  return strlength;
}

int putint(int value)
{
  char * ptr=((void *)0);
  char * low=((void *)0);
  char * rc=((void *)0);
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
  return puts(rc);
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
  return puts(rc);
}

int long2hex(unsigned long value)
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
  return puts(rc);
}

int putlong(unsigned long value)
{
  char * ptr=((void *)0);
  char * low=((void *)0);
  char *rc=((void *)0);
  rc = ptr;
  low =ptr;
  do
  {
    // Modulo is negative for negative value. This trick makes abs() unnecessary.
    *ptr++ = "0123456789"[value%10];
    value /= 10;
  } while ( value );
  
  *ptr-- = '\0';
  while ( low < ptr )
  {
    char tmp = *low;
    *low++ = *ptr;
    *ptr-- = tmp;
  }
  return puts(rc);
}

int printf(const char* format, ...) 
{
  const char* str;
	va_list parameters;

  int total=0;
  va_start(parameters, format);
  str = format;
  while(*str != '\0')
  {
    if(*str != '%')
    {
      total+=putchar(*str++);
      continue;
    }
    str++;
    switch( *str )
    {
      case 'c': total+=putchar(va_arg(parameters, int));
                break;

      case 'd': total+=putint(va_arg(parameters, int));
                break;

      case 's': total+=puts(va_arg(parameters, char*));
                break;

      case 'x': total+=int2hex(va_arg(parameters, int));
                break;
      
      case 'l': if(*(++str)=='d')
                {
                  total+=putlong(va_arg(parameters, unsigned long));
                  break;
                }

      case 'p': total+=long2hex(va_arg(parameters, unsigned long));
                break;

      case '%': total+=putchar('%');
                break;
    }
    str++;
  }
  va_end(parameters);
  update_cursor(position/COLUMN, position%COLUMN);
  return total;
}

void printtime(unsigned char hour, unsigned char minute, unsigned char seconds)
{
  int color = 0x07;
  //hour = hour-4;
	volatile char *video = (volatile char*)0xB8F90;
 
  if(hour >= 4)
    hour = (hour)-4;
  else
    hour = hour + 20;

  char a = (hour/10) + '0';
  *(video++) = a;
	*video++ = color;
  a = (hour%10)+'0';
  *(video++) = a;
	*video++ = color;

  a = ':';
  *(video++) = a;
	*video++ = color;
  
  a = (minute/10)+'0';
  *(video++) = a;
	*video++ = color;
  a = (minute%10)+'0';
  *(video++) = a;
	*video++ = color;
  
  a = ':';
  *(video++) = a;
	*video++ = color;
  
  a = (seconds/10)+'0';
  *(video++) = a;
	*video++ = color;
  a = (seconds%10)+'0';
  *(video++) = a;
	*video++ = color;
}
