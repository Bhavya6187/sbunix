#ifndef _STDIO_H
#define _STDIO_H

//#include <unistd.h>

inline void outb( unsigned short port, unsigned char val );
int printf(const char *format, ...);
int putchar(char a);
int puts(char* str);
int scanf(const char *format, ...);
int clrscr();
int scrollup(int dist);
int putint(int value);
int int2hex(int value);
void update_cursor(int row, int col);
int long2hex(unsigned long value);
int putlong(unsigned long value);
#endif
