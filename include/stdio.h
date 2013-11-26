#ifndef _STDIO_H
#define _STDIO_H


#include<defs.h>

extern uint64_t video_vm;

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
void printtime(unsigned char hour, unsigned char minute, unsigned char seconds);
void printtoside(char hour);
uint64_t sys_putint(int a);
int puts_user(char* str, int n);
inline void outb( unsigned short port, unsigned char val )
{
    __asm volatile( "outb %0, %1": : "a"(val), "Nd"(port) );
}

inline unsigned char inb( unsigned short port )
{
      unsigned char ret;
          __asm volatile( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
              return ret;
}
#endif
