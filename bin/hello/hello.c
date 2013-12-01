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
   uint64_t ret=0;
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
  ret = u_printf("hey %s %p\n",test_malloc1,test_malloc1);
/*  struct ad *ad1 = (struct ad*) malloc(sizeof(struct ad));
  ad1->t1 = 123213213;
  ad1->t2 = 99;
  ad1->t3 = 2324564;
  ad1->t4 = 23244566;
  ad1->t5 = 23244566;
  ad1->t6 = 0;
  ad1->t7 = 2324456;
  ad1->t8 = 2324456;
  u_printf("%d:%d:%d:\n%d:%d:%d\n:%d:%d", ad1->t1,ad1->t2,ad1->t3,ad1->t4,ad1->t5,ad1->t6,ad1->t7,ad1->t8);*/
  uint64_t tester;
  u_scanf("%x",(uint64_t)&tester);
  ret = u_printf("scanf gave us %p",tester);
  
  char test_scan[6];
  u_scanf("%s",(uint64_t)&test_scan);
  ret = u_printf("scanf gave us %s",test_scan);



  while(1);
	return 0;
}
