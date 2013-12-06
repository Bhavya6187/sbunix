# include <stdio.h>
# include <syscall.h>
# include <libc/malloc.h>
#include <stdarg.h>
#include <defs.h>

int main(int argc, char* argv[]) 
{
  while(1);
  //  u_printf("%d ", getpid());
  /*int temp =0, i,j; 
  temp++;
  //char filename[15] = "bin/world";
  char **params=NULL;
  for(i=0;i<5;i++)
    params[i] = (char*)malloc(sizeof(char)*10);
  params[0]="dushyant\0";
  params[1]="duant\0";*/
 /*
  int ret1 = fork(); 
  if(ret1==0)
  {
    u_printf("First Child:%d:\n", ret1);
    //yield();
    while(1);
    {
      for(i=0;i<500;i++)
        for(j=0;j<500;j++)
          ;
      //u_printf("In child via timer scheduling :) :: %p\n", getpid());
      u_printf("L %p ", getpid());
    }
  }
  else
  {
    if(ret1>1)
    {
      u_printf("First Parent:%d:\n", ret1);
      u_printf("Returned from First Parent:%d:\n", ret1);

       int ret2;
      ret2 = fork();
      if(ret2==0)
      {
        execvp("bin/world", params);
        u_printf("Child!!!:%d:\n", ret2);
        while(1)
          u_printf("Last child");
      }
      else
      {
        u_printf("Parent i am awesome !!!:%d:\n", ret2);
        //while(1);
        yield();
      }*/
    /* while(1);
    {
      for(i=0;i<500;i++)
        for(j=0;j<500;j++)
          ;
      //u_printf("In parent via timer scheduling :) :: %p\n", getpid());
      u_printf("C %p ", getpid());
    }

    }
  }
*/
  /*    while(1)
      {
        for(i=0;i<500;i++)
        for(j=0;j<500;j++)
          ;
        u_printf("In parent via timer scheduling :) \n");
      }
      //u_printf("LALALALLALALALALLALA !! ... \n");
    }

  }*/

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
