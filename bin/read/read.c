#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <libc/dir.h>
#include <stdarg.h>
#include<defs.h>
int main(int argc, char* argv[]) 
{

  char* ch = "etc/abc.sh";

  u_printf("Opening ch %s",ch);
  int fd = open(ch);
  if(fd==-1)
    u_printf("File not found");
  else
  {
    u_printf("\nFile has fd %d",fd);
    char st[20];
    int i = 0;

    for(i = 0; i<10;i++){
      read(fd,1,st);
        u_printf("char at %d - %c\n",i,st[0]);
    }
  }
   close(fd) ;
}
