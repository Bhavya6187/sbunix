# include <stdio.h>
# include <syscall.h>
# include <libc/malloc.h>
#include <stdarg.h>
#include <defs.h>

int main(int argc, char* argv[]) 
{
  int ret1 = fork(); 
 
  if(ret1==0)
  {
    u_printf("First Child:%d:\n", ret1);
    u_printf("Child going to sleep for 60 secs :) %d\n", getpid());
    sleep(60);
    u_printf("Child woke up :) %d\n", getpid());
  }
  else
  {
    
    u_printf("Will wait for child to wake up :) \n");
    waitpid(ret1);
    u_printf("In parent Child woke up :) :: %d\n", getpid());

  }

	return 0;
}
