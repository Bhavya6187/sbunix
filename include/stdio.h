#ifndef _STDIO_H
#define _STDIO_H


#include<defs.h>

extern uint64_t video_vm;

int user_putint(int value, char* buffer, int total);
int user_long2hex(unsigned long value, char* buffer, int total);
int user_putlong(unsigned long value, char* buffer, int total);
int user_putchar(char a, char* buffer , int total);
int user_int2hex(int value, char* buffer, int total);
int user_puts(char* str, char* buffer, int total);
int printf(const char *format, ...);
uint64_t u_printf(const char *format, ...);
uint64_t u_scanf(const char *format, uint64_t ptr);
int putchar(char a);
int puts(char* str);
int scanf(const char *format,uint64_t ptr );
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
uint64_t stderr(const char* fmt, ...) ;
int write(int fd, volatile char* str, int n);
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
