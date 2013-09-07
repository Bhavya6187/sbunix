#ifndef _STDIO_H
#define _STDIO_H
#endif

//#include <unistd.h>

int position=0;
int scanf(const char *format, ...);
int printf(const char* str) {
	return 0;
}

int putchar(char a)
{
	int color = 0x07;
	volatile char *video = (volatile char*)0xB8000;
  *(video++ + 2*position) = a;
	*video++ = color;
  position++;
  return 0;
}

