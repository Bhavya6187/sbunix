# include <stdio.h>
# include <syscall.h>
# include <libc/malloc.h>
#include <stdarg.h>
#include <defs.h>

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
  int temp =0, i,j, res=0; 
  temp++; res++;
  //char filename[25] = "bin/test_fork";
  //char filename[25] = "bin/test_timer";
  char filename[25] = "bin/test_fork_timer";
  u_printf("Checking for TIMER ..>> This is INIT.\n");
  
  int ret1;
  ret1 = fork();
  if(ret1==0)
  {
    u_printf("In child return from exec()\n");
    //sleep(10);
    //execve(filename);
    char f[10] = "bin/ps";
    execve(f);
    /*int t=0;
    while(t<100000)
    {
      u_printf("%p ", getpid());
      t++  ;
    }*/
    //exit(0);
  }
  else
  {
    waitpid(ret1);
    //sleep(100);
    while(1);
    while(1)
      u_printf("%p ", getpid());
  }

  //char params[5][10] = {"dushyant", "goyal", "\0", "\0", "\0"};
  char **params=NULL;
  for(i=0;i<5;i++)
  {
    params[i] = (char*)malloc(sizeof(char)*10);
  }
  params[0]="dushyant\0";
  params[1]="duant\0";


  //char filename[25] = "bin/world";
  ret1 = fork(); 
  if(ret1==0)
  {
    //temp = 20;
    u_printf("HELLO>> CHILD fork return:%d:\n", ret1);
    u_printf("This is child !! \n");
    //__syscall1(SYSCALL_EXIT, 0);
    //exit(0);
    //ret1 = __syscall1(SYSCALL_EXECVE,(uint64_t)(filename));
    u_printf("In child return from exec()\n");
    execve(filename);
    //execvp(filename, params);
    u_printf("In child return from exec()\n");
    while(1);
      u_printf("Hiiii\n");
  }
  else
  {
    if(ret1>1)
    {
      u_printf("HELLO>>> Parent fork return:%d:\n", ret1);
      u_printf("This is parent!! \n");
      //ret1 = __syscall0(SYSCALL_SCHEDULER);
      //yield();
      //while(1);
      //u_printf("Returning back from schedule in parent!! \n");
      u_printf("Waiting for its child to ext()\n");
      //res = waitpid(ret1);
      //res = waitpid(ret1);
      //u_printf("Returning back after waiting for the child exits .. pid child=%p %d!! \n",ret1, res);
      //  u_printf("Fuck UUUUU\n");
      yield();
      yield();
      while(1)
        u_printf("lala");
      yield();
      while(1);
      //ret1 = __syscall0(SYSCALL_SCHEDULER);
      while(1)
      {
        for(i=0;i<500;i++)
        for(j=0;j<500;j++)
          ;
        //u_printf("In Parent after schedule\n");
      }
      //u_printf("LALALALLALALALALLALA !! ... \n");
    }

  }

  /*u_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"); 
  while(1);*/
  
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
