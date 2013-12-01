#include <stdarg.h>
#include <stdio.h>
#include <syscall.h>
#include<defs.h>
#include<libc/malloc.h>

uint64_t u_scanf(const char* fmt, uint64_t ptr) 
{
  uint64_t ret = 0;
  ret=__syscall2(SYSCALL_SCANF,(uint64_t)fmt,ptr);
  return ret;
}
