#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <stdarg.h>
#include <defs.h>

int main(int argc, char* argv[]) 
{
  ps_list();
  while(1);
  return 0;
}
 
