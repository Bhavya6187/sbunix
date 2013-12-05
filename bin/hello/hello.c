# include <stdio.h>
# include <syscall.h>
# include <libc/malloc.h>
#include <stdarg.h>
#include<defs.h>

struct ad
{
  uint64_t t1;
  uint64_t t2;
  uint64_t t3;
  uint64_t t4;
  uint64_t t5;
  uint64_t t6;
  uint64_t t7;
  uint64_t t8;
};

int main(int argc, char* argv[]) 
{
  /*uint64_t ret=0;
  char s[5] = "abcd";
  ret= u_printf("bhavya\n");
  u_printf("ret=%d\n", ret);
  __syscall1(SYSCALL_PUTINT,ret);
  ret = u_printf("hi i am not bhavya\n");
  u_printf("ret=%d\n", ret);
  __syscall1(SYSCALL_PUTINT,ret);
  ret = u_printf("hey %d %c %s %d\n",50,'a',s,200);
  u_printf("ret=%d\n", ret);
  __syscall1(SYSCALL_PUTINT,ret);
  //u_printf("hi %d %c %s %d\n",50,'a',s,200);
  ret = __syscall2(SYSCALL_PUTS,(uint64_t)"xkcd\n",5);
  u_printf("ret=%d\n", ret);
  __syscall1(SYSCALL_PUTINT,ret);
  char* test_malloc = (char*)malloc(4);
  test_malloc = "abc";
  ret = u_printf("hey %s %p\n",test_malloc,test_malloc);
  char* test_malloc1 = (char*)malloc(1600);
  test_malloc1 = "abcdefghijklmnop";
  ret = u_printf("hey %s %p\n",test_malloc1,test_malloc1);*/
  //u_printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); 
  //u_printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); 
  //u_printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); 
  //u_printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); 
  //int ret1 = __syscall0(SYSCALL_FORK);
  int ret1 = fork(); 
  int temp =0, i,j; 
  temp++;
  char filename[15] = "bin/world";
  if(ret1==0)
  {
    //temp = 20;
    //u_printf("fork return:%d:", ret1);
    //u_printf("This is child !! \n");
    //__syscall1(SYSCALL_EXIT, 0);
    //ret1 = __syscall1(SYSCALL_EXECVE,(uint64_t)(filename));
    execve(filename);
    //u_printf("In child return from exec()\n");
    while(1);
      u_printf("Hiiii\n");
  }
  else
  {
    if(ret1>1)
    {
      u_printf("fork return:%d:", ret1);
      u_printf("This is parent!! \n");
      ret1 = __syscall0(SYSCALL_SCHEDULER);
      //while(1);
      //  u_printf("Fuck UUUUU\n");
      while(1);
      {
        for(i=0;i<500;i++)
        for(j=0;j<500;j++)
          ;
        u_printf("%c", 'D');
      }
      //u_printf("LALALALLALALALALLALA !! ... \n");
    }

  }

  //u_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"); 
  
  /*
  struct ad *ad1 = (struct ad*) malloc(sizeof(struct ad));
  struct ad *ad1 = (struct ad*) malloc(sizeof(struct ad));
  ad1->t1 = 123213213;
  ad1->t2 = 99;
  ad1->t3 = 2324564;
  ad1->t4 = 23244566;
  ad1->t5 = 23244566;
  ad1->t6 = 0;
  ad1->t7 = 2324456;
  ad1->t8 = 2324456;
  u_printf("%d:%d:%d:\n%d:%d:%d\n:%d:%d", ad1->t1,ad1->t2,ad1->t3,ad1->t4,ad1->t5,ad1->t6,ad1->t7,ad1->t8);*/
  
  /*int tester;
  u_scanf("%d",(uint64_t)&tester);
  ret = u_printf("scanf gave us %d",tester);
  
  char test_scan[6];
  u_scanf("%s",(uint64_t)&test_scan);
  ret = u_printf("scanf gave us %s",test_scan);
  */
  //while(1);
  /*uint64_t i=0, j=0;
  
  while(1)
  {
    for(i=0;i<500;i++)
    for(j=0;j<500;j++)
      ;
    u_printf("%c", 'D');
  }*/
	return 0;
}
