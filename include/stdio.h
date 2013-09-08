#ifndef _STDIO_H
#define _STDIO_H

//#include <unistd.h>

//int position=0;
//int printf(const char *format, ...);
int printf(const char *format);
int putchar(char a);
int puts(char* str);
int scanf(const char *format, ...);
int clrscr();
int putint(int value);
int int2hex(int value);
void update_cursor(int row, int col);
#endif
