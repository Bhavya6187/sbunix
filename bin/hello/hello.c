# include <stdio.h>
# include <syscall.h>

#include <stdarg.h>
#include<defs.h>
/*int user_putint(int value, char* buffer, int total)
{
  char ptr[30]={'\0'};
  char rc[30]={'\0'};
  int index = 0;
  int neg = 0;
  int j = 0;
  int i =0;
  char vals[10] = "0123456789";
  if ( value < 0 )
  {
    value*=-1;
    neg = 1;
  }
  do
  {
    // Modulo is negative for negative value. This trick makes abs() unnecessary.
    ptr[index] = vals[value%10];
    value /= 10;
    index++;
  } while ( value );
  if(neg)
    ptr[index]='-';
  else
    index--;
  while ( index >= 0 )
  {
    rc[j]=ptr[index];
    j++;
    index--;
  }
  for(i = 0 ; i < j ; i++)
  {
    buffer[total+i] = rc[i];
  }
  //__syscall2(SYSCALL_PUTS,(uint64_t)rc,i);
  return i;
}

int user_long2hex(unsigned long value, char* buffer, int total)
{
  char ptr[40]={'a'};
  char rc[40]={'a'};
  int i =0;
  int index =0, j= 0; 
  do
  {
    int temp = value & 0x0f;
    if(temp < 10)
        ptr[index] = temp + '0';
     else
        ptr[index] = (temp) - 10 + 'A';
     value = value >> 4;
     index++;

  } while ( value );
  
  ptr[index] = 'x';
  index++;
  ptr[index] = '0';
  while ( index >= 0 )
  {
    rc[j]=ptr[index];
    j++;
    index--;
  }
  for(i = 0 ; i < j ; i++)
  {
    buffer[total+i] = rc[i];
  }
  //__syscall2(SYSCALL_PUTS,(uint64_t)rc,i);
  return i;
}

int user_putlong(unsigned long value, char* buffer, int total)
{
  char ptr[30]={'a'};
  char rc[30]={'a'};
  int index = 0;
  int neg = 0;
  int j = 0, i = 0;
  char vals[10] = "0123456789";
  if ( value < 0 )
  {
    value*=-1;
    neg = 1;
  }
  do
  {
    // Modulo is negative for negative value. This trick makes abs() unnecessary.
    ptr[index] = vals[value%10];
    value /= 10;
    index++;
  } while ( value );
  if(neg)
    ptr[index]='-';
  else
    index--;
  while ( index >= 0 )
  {
    rc[j]=ptr[index];
    j++;
    index--;
  }
  for(i = 0 ; i < j ; i++)
  {
    buffer[total + i] = rc[i];
  }
  //__syscall2(SYSCALL_PUTS,(uint64_t)rc,i);
  return i;
}


int user_int2hex(int value, char* buffer, int  total)
{
  char ptr[32]={'a'} ;
  char rc[32]={'a'};
  int index =0, j= 0; 
  int i =0;
  do
  {
    int temp = value & 0x0f;
    if(temp < 10)
        ptr[index] = temp + '0';
     else
        ptr[index] = (temp) - 10 + 'A';
     value = value >> 4;
     index++;

  } while ( value );
  
  ptr[index] = 'x';
  index++;
  ptr[index] = '0';
  while ( index >= 0 )
  {
    rc[j]=ptr[index];
    j++;
    index--;
  }
  for(i = 0 ; i < j ; i++)
  {
    buffer[total+i] = rc[i];
  }

  //__syscall2(SYSCALL_PUTS,(uint64_t)rc,i);
  return i;
}

int user_puts(char* str,char* buffer, int total)
{
  int i=0;
  while(str[i] != '\0')
  {
    buffer[i+total]=str[i];
    i++;
  }
 // __syscall2(SYSCALL_PUTS,(uint64_t)str,i);
  return i;
}
int user_putchar(char a, char* buffer , int total)
{
  buffer[0+total]=a;
  return 1;
}
uint64_t u_printf(const char* fmt, ...) 
{
	va_list parameters;
  char buffer[100] = {'\0'}; //= (char*)malloc(512) ;
  volatile int total=0;
  const char* str = fmt;
  va_start(parameters, fmt);
  str = fmt;
  while(*str != '\0')
  {
    if(*str != '%')
    {
      buffer[total] = *str;
      str++;
      total++;

      //__syscall1(SYSCALL_PUTINT,total);
      continue;
    }
    str++;
    switch( *str )
    {
      case 'c': total+=user_putchar(va_arg(parameters, int), buffer,total);
                break;

      case 'd': total+=user_putint(va_arg(parameters, int),buffer,total);
                break;

      case 's': total+=user_puts(va_arg(parameters, char*), buffer,total);
                break;

      case 'x': total+=user_int2hex(va_arg(parameters, int), buffer,total);
                break;
      
      case 'l': if(*(++str)=='d')
                {
                  total+=user_putlong(va_arg(parameters, unsigned long), buffer,total);
                  break;
                }

      case 'p': total+=user_long2hex(va_arg(parameters, unsigned long), buffer,total);
                break;

      case '%': total+=user_putchar('%', buffer,total);
                break;
    }
    str++;
  }
  va_end(parameters);
  buffer[total] = '\0';
  total++;

  __syscall2(SYSCALL_PUTS,(uint64_t)buffer,total);

  return (uint64_t)total;
}*/
int main(int argc, char* argv[]) 
{

	//sys_putint(6187);
//  int a = 6187;
//  __syscall1(SYSCALL_PUTINT,a);
//  __syscall1(SYSCALL_PUTINT,986);
 // __syscall1(SYSCALL_PUTINT,1234);
//  __syscall1(SYSCALL_PUTINT,4321);
 char s[4] = "abcd";

//  __syscall2(SYSCALL_PUTS,(uint64_t)s,4);
 // __syscall2(SYSCALL_PUTS,(uint64_t)"xkcd",4);
 // char s2[4] = "xxxx";

 // __syscall2(SYSCALL_PUTS,(uint64_t)s2,4);
 //u_printf("hello",200);
 // u_printf("hello %d %c %s %d\n",50,'a',s,200);
  u_printf("hey %d %s %c %p\n",6187,s,'a',(uint64_t)s);
 //ret = u_printf("hey bitch %d\n",1234);
  u_printf("bhavya %d\n",6187);
  u_printf("hi i am not bhavya\n");
  //u_printf("hey %d %c %s %d\n",50,'a',s,200);
  //u_printf("hi %d %c %s %d\n",50,'a',s,200);
//  __syscall2(SYSCALL_PUTS,(uint64_t)"xkcd",4);
  while(1);
	return 0;
}
