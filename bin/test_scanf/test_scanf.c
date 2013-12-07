#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <stdarg.h>
#include <defs.h>

int main(int argc, char* argv[]) 
{
  int ret, tester;
  u_scanf("%d",(uint64_t)&tester);
  ret = u_printf("scanf gave us %d",tester);
  
  char test_scan[6];
  u_scanf("%s",(uint64_t)&test_scan);
  ret = u_printf("scanf gave us %s",test_scan);
  u_printf("%d", ret);
  
  return 0;
}
