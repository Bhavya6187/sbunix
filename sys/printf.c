#include <stdarg.h>
#include <stdio.h>
#include<defs.h>

//#include <unistd.h>
#define ROW 25
#define COLUMN 80
#define VIDEO_VM 0xFFFF800000000000
int position=0;
uint64_t video_vm = 0xB8000;
int scanf(const char *format, ...);

extern uint64_t physfree;
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
	//volatile char *video = (volatile char*)(0xB8000);
	//volatile char *video = (volatile char*)(VIDEO_VM | 0xB8000);
	volatile char *video = (volatile char*)(video_vm);
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
	//volatile char *video = (volatile char*)(VIDEO_VM | 0xB8000);
	volatile char *video = (volatile char*)(video_vm);
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
	volatile char *video = (volatile char*)(video_vm);
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
  char ptr[30];
  char rc[30];
  int index = 0;
  int neg = 0;
  int j = 0;
  char vals[10] = "0123456789";
  if ( value < 0 )
  {
    value*=-1;
    neg = 1;
  }
  do
  {
    // Modulo is negative for negative value. This trick makes abs() unnecessary.
    ptr[index] = vals[value%10];
    value /= 10;
    index++;
  } while ( value );
  if(neg)
    ptr[index]='-';
  else
    index--;
  while ( index >= 0 )
  {
    rc[j]=ptr[index];
    j++;
    index--;
  }
  rc[j]= '\0';
  return puts(rc);
}

int int2hex(int value)
{
  char ptr[16];
  char rc[16];
  int index =0, j= 0; 
  do
  {
    int temp = value & 0x0f;
    if(temp < 10)
        ptr[index] = temp + '0';
     else
        ptr[index] = (temp) - 10 + 'A';
     value = value >> 4;
     index++;

  } while ( value );
  
  ptr[index] = 'x';
  index++;
  ptr[index] = '0';
  while ( index >= 0 )
  {
    rc[j]=ptr[index];
    j++;
    index--;
  }
  return puts(rc);
}

int long2hex(unsigned long value)
{
  char ptr[16];
  char rc[16];
  int index =0, j= 0; 
  do
  {
    int temp = value & 0x0f;
    if(temp < 10)
        ptr[index] = temp + '0';
     else
        ptr[index] = (temp) - 10 + 'A';
     value = value >> 4;
     index++;

  } while ( value );
  
  ptr[index] = 'x';
  index++;
  ptr[index] = '0';
  while ( index >= 0 )
  {
    rc[j]=ptr[index];
    j++;
    index--;
  }
  return puts(rc);
}

int putlong(unsigned long value)
{
  char ptr[30];
  char rc[30];
  int index = 0;
  int neg = 0;
  int j = 0;
  char vals[10] = "0123456789";
  if ( value < 0 )
  {
    value*=-1;
    neg = 1;
  }
  do
  {
    // Modulo is negative for negative value. This trick makes abs() unnecessary.
    ptr[index] = vals[value%10];
    value /= 10;
    index++;
  } while ( value );
  if(neg)
    ptr[index]='-';
  else
    index--;
  while ( index >= 0 )
  {
    rc[j]=ptr[index];
    j++;
    index--;
  }
  rc[j]= '\0';
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
	//volatile char *video = (volatile char*)(0xB8F90);
	volatile char *video = (volatile char*)(video_vm + 0x00F90);
	//volatile char *video = (volatile char*)(VIDEO_VM | 0xB8F90);
 
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

void printtoside(char a)
{
  int color = 0x07;
	//volatile char *video = (volatile char*)(0xB8F8C);
	volatile char *video_2 = (volatile char*)(video_vm + 0x00F8C);
	//volatile char *video_2 = (volatile char*)(video | 0xB8F8C);
  //update_cursor(70, 25);
  *(video_2++) = a;
	*video_2++ = color;
}
