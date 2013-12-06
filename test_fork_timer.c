#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <stdarg.h>
#include <defs.h>

int main(int argc, char* argv[]) 
{
  int temp =0, i,j, res=0; 
  temp++; res++;
  u_printf("Checking for TIMER ..>> This is INIT.\n");
  
  int ret1;
  ret1 = fork();
  if(ret1==0)
  {
    int ret2=fork();
    if(ret2==0)
    {
      while(1)
      { 
        for(i=0;i<500;i++)
        for(j=0;j<500;j++)
          ;
        u_printf("%p", getpid());
      }
    }
    else
    {
      while(1)
      { 
        for(i=0;i<500;i++)
        for(j=0;j<500;j++)
          ;
        u_printf("%p", getpid());
      }
    }
  }
  else
  {
    while(1)
    {
      for(i=0;i<500;i++)
      for(j=0;j<500;j++)
          ;
      u_printf("%p", getpid());
    }
  }
	return 0;
}
