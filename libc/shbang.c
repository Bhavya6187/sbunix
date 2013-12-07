#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <libc/dir.h>
#include <stdarg.h>
#include<defs.h>

int check_shbang(char* path)
{
  int fd = open(path);
  char st[20];
  if(fd==-1)
    return 0;
  else
  {

    read(fd,2,st);
  }
   close(fd);
  if(st[0] == '#' && st[1]=='!' )
   return 1;
  return 0;
}
int read_shbang(char* path, char**params)
{
  int fd = open(path);
  char st[1];
  int i = 0, j = 0;
  if(fd==-1)
    return 0;
  read(fd,1,st);
  read(fd,1,st);
  read(fd,1,st);
  //char params[10][20];
  int check = 1;
  while(check)
  {      
    i=0;
    while(st[0]!='\n')
    {
       int num = read(fd,1,st);
       if(num == -2)
       {
         check = 0;
         params[j][i]='\0';
         break;
       }
       if(st[0]!='\n')
       params[j][i] = st[0];
       i++;
    }
    st[0]=1;
    params[j][i]='\0';
    j++;
  }
  close(fd);
  return j-1;
}
