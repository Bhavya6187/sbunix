#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <stdarg.h>
#include <defs.h>
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
  struct ad *ad1 = (struct ad*) malloc(sizeof(struct ad));
  ad1->t1 = 123213213;
  ad1->t2 = 99;
  ad1->t3 = 2324564;
  ad1->t4 = 23244566;
  ad1->t5 = 23244566;
  ad1->t6 = 0;
  ad1->t7 = 2324456;
  ad1->t8 = 2324456;
  u_printf("%d:%d:%d:\n%d:%d:%d\n:%d:%d", ad1->t1,ad1->t2,ad1->t3,ad1->t4,ad1->t5,ad1->t6,ad1->t7,ad1->t8);
 
  return 0;

}	
