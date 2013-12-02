#include <stdarg.h>
#include <stdio.h>
#include<defs.h>
#include<sys/irq.h>


volatile char buf[128];

int get_int(int* ptr )
{
  int i = 0;
  int ret = 0;
  while(buf[i] != '\0')
  {
    if( buf[i]<='9' && buf[i] >= '0' )
    {
      ret =(buf[i]-'0')+(ret*10);
      i++;
    }
    else
    {
      return -1;
    }
  }
  *ptr=ret;
  return 1;
}

int get_hex(uint64_t* ptr )
{
  int i = 0;
  int ret = 0;
  while(buf[i] != '\0')
  {
    if( buf[i]<='9' && buf[i] >= '0' )
      ret =(buf[i]-'0')+(ret*16);
    else
    { 
      if( buf[i]<='F' && buf[i] >= 'A' )
         ret =(buf[i]-'A'+10)+(ret*16);
      else{
        if( buf[i]<='f' && buf[i] >= 'a' )
          ret =(buf[i]-'a'+10)+(ret*16);
        else
          return -1;
      }
    }

    i++;
  }
  *ptr=ret;
  return 1;
}
int scanf(const char *format, uint64_t ptr)
{
  int i=0,ret = 0;
  printf("In scanf\n");
  for(i = 0; i <= kbuf_index ; i++)
  {
     kbuf[i]='\0';
     buf[i]='\0';
  }
  kbuf_index = -1;
  int prev_val = kbuf_index;
  while(1)
  {
     if(prev_val < kbuf_index)
     {
         printf("buffer index is %d with val = %c\n",kbuf_index,kbuf[kbuf_index]);
         prev_val = kbuf_index;
     }
     if(kbuf[kbuf_index] == '\n')
     {
        for(i = 0; i < kbuf_index ; i++)
        {
          buf[i]=kbuf[i];
          printf("buffer is = %c at %d",buf[i],i);
          kbuf[i]='\0';
        }
        buf[kbuf_index]='\0';
        printf("buffer is = %s",buf);
        kbuf_index = 0;
        if(format[0] == '%' && format[1]=='d')
          ret = get_int((int*)ptr);
        if(format[0] == '%' && format[1]=='x')
          ret = get_hex((uint64_t*)ptr);
        if(format[0] == '%' && format[1]=='s')
        {
          char* str = (char*)ptr;
          i=0;
          while(buf[i] != '\0')
          {
            str[i] =buf[i];
            i++;
          }
          ret = 1;
        }
        printf("out of scanf\n");
        return ret;
     }
  }
  printf("out of scanf from outer loop");
  return ret;;
}
