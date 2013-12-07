#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <stdarg.h>
#include <defs.h>

int main(int argc, char* argv[]) 
{
  char *a = NULL;
  u_printf("We will try to access NULL");
  *a = 'D';
  u_printf("%c", a);
  return 0;
}
 
