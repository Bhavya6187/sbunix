# include <stdio.h>
# include <syscall.h>

int main(int argc, char* argv[]) 
{
	//sys_putint(6187);
  int a = 6187;
  __syscall1(SYSCALL_PUTINT,a);
  //__syscall1(SYSCALL_PUTINT,1234);
  //__syscall1(SYSCALL_PUTINT,4321);
  char s[4] = "abcd";

  __syscall2(2,(uint64_t)s,4);
  while(1);
	return 0;
}
