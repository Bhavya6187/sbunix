#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <defs.h>

#define SYSCALL_PROTO(n) static __inline uint64_t __syscall##n
#define SYSCALL_PUTINT 1
#define SYSCALL_PUTS 2 

SYSCALL_PROTO(0)(uint64_t n) {
	return 0;
}

SYSCALL_PROTO(1)(uint64_t n, uint64_t a1) {
  __asm__ volatile("int $80;"::"a"((uint64_t)n), "b" ((uint64_t)a1 ));
	return 0;
}

SYSCALL_PROTO(2)(uint64_t n, uint64_t a1, uint64_t a2) {
  __asm__ volatile("movq %0,%%rax;":"=m"(n)::);
  __asm__ volatile("movq %0,%%rbx;":"=m"(a1)::);
  __asm__ volatile("movq %0,%%rcx;":"=m"(a2)::);
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
