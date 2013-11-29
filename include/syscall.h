#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <defs.h>

#define SYSCALL_PROTO(n) static __inline uint64_t __syscall##n
#define SYSCALL_PUTINT 1
#define SYSCALL_PUTS 2 
#define SYSCALL_MALLOC 4

SYSCALL_PROTO(0)(uint64_t n) {
	return 0;
}

SYSCALL_PROTO(1)(uint64_t n, uint64_t a1) {
  __asm volatile( "movq %0,%%rax\n\t"
                  "movq %1,%%rbx\n\t"
                  ::"r"(n),"r"(a1):"memory");
  __asm__ volatile("int $80;");
	return 0;
}

SYSCALL_PROTO(2)(uint64_t n, uint64_t a1, uint64_t a2) {
/*   uint64_t ret;
   __asm volatile("movq %1,%%rax;"
                  "movq %2,%%rbx;"
                  "movq %3, %%rcx;"
                  "int $80;"
                  "movq %%rax,%0;"
                  :"=r"(ret)
                  :"r"(n),"r"(a1),"r"(a2)
                  :"memory"
                  );
   return ret;
}*/
   __asm volatile( "movq %0,%%rax\n\t"
                   "movq %1,%%rbx\n\t"
                   "movq %2,%%rcx\n\t"
                  ::"r"(n),"r"(a1),"r"(a2):"memory");
  __asm__ volatile("int $80;");

	return 0;
}

SYSCALL_PROTO(3)(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
	return 0;
}

SYSCALL_PROTO(4)(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4) {
	return 0;
}

#endif
