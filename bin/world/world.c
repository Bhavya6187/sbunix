# include <stdio.h>
# include <syscall.h>
# include <libc/malloc.h>
#include <stdarg.h>
#include<defs.h>

int main(int argc, char* argv[]) 
{

  int i=1;
  i++;
  u_printf("dushyant says hi !!\n");
  u_printf("ret=%d\n", 34);
  while(1);
  
	return 0;
}
