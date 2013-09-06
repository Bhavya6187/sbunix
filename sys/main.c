#include <defs.h>
#include <stdio.h>
void putchar(char a)
{
	int color = 0x07;
	volatile char *video = (volatile char*)0xB8000;
	*video++ = a;
	*video++ = color;
}
void start(void* modulep, void* physbase, void* physfree)
{
	volatile char *video = (volatile char*)0xB8000;
	//char* string = "abvkjajbvjabvkvknkanknvknekvfnnknvljeflalqnnav   nvbbknrbnanebnakjnba kbnelanmbvaenblanl ojkbnlbnlsenbkllsbnlalbnalbla   ngvnljnjbnajlbnjnajbnljanbljanjbnlanvanblnlnslblsnblnlkafjegfoj";
	int color = 0x07;
	int i;
	//:wchar ch=' ';
	video = (volatile char*)0xB8000;
	for(i=0 ;i < 2000 ; i++)
	{
		*video++ = NULL;
		*video++ = color;
	}
	putchar('a');
// kernel starts herie
}
#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;

void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	volatile register char *rsp asm ("rsp");
//	volatile register char *temp1, *temp2;
	loader_stack = (uint32_t*)rsp;
	rsp = &stack[INITIAL_STACK_SIZE];
	start(
		(char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase,
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
/*	for(
		temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8050;
		*temp1;
		temp1 += 1, temp2 += 2
	) *temp2 = *temp1;
*/	while(1);
}
