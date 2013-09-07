#ifndef _STDIO_H
#define _STDIO_H

//#include <unistd.h>

//int position=0;
//int printf(const char *format, ...);
int printf(const char *format);
int putchar(char a);
int scanf(const char *format, ...);


#endif

int printf(const char* str) {
	return 0;
}

int putchar(char a)
{
  int position =0;
	int color = 0x07;
	volatile char *video = (volatile char*)0xB8000;
  *(video + position) = a;
	*video++ = color;
  position++;
  return 0;
}

