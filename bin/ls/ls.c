#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <libc/dir.h>
#include <stdarg.h>
#include<defs.h>
#include<libc/bash.h>

int main(int argc, char* argv[]) 
{
  
  struct dirent* dir = (struct dirent*)malloc(sizeof(struct dirent* ));
  DIR* directory;
  if(argv[0]==NULL)
    directory = u_opendir("bin/");
  else
    directory= u_opendir(argv[0]);


  dir = u_readdir(directory);
  
  while(dir!=NULL){
  u_printf("%s  ",dir->d_name);
  dir = u_readdir(directory);
  }

  //exit(0);
  //while(1);
  return 0;
}
