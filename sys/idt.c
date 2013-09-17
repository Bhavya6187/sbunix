#include <sys/idt.h>

/* adapted from Chris Stones, shovelos */

#define MAX_IDT 256

struct idt_entry {
  uint16_t offset1;
  uint16_t target_selector;
  uint16_t flags;
//  IST | RESERVED | TYPE | ZERO | DPL0| P,
  uint16_t offset2;
  uint32_t offset3;
  uint32_t reserved;
}__attribute__((packed));

struct idt_entry idt[MAX_IDT];

struct idtr_t {
	uint16_t size;
	uint64_t addr;
}__attribute__((packed));

static struct idtr_t idtr = {
	sizeof(idt),
	(uint64_t)idt
};

void _x86_64_asm_lidt(struct idtr_t* idtr);
void reload_idt() {
	_x86_64_asm_lidt(&idtr);
}