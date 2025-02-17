#include "kernel.h"
#include "common.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

extern char __bss[], __bss_end[], __stack_top[];

struct sbiret sbi_call(
	long arg0,
	long arg1,
	long arg2,
	long arg3,
	long arg4,
	long arg5,
	long fid,
	long eid
) {
	register long a0 __asm__("a0") = arg0;
	register long a1 __asm__("a1") = arg1;
	register long a2 __asm__("a2") = arg2;
	register long a3 __asm__("a3") = arg3;
	register long a4 __asm__("a4") = arg4;
	register long a5 __asm__("a5") = arg5;
	register long a6 __asm__("a6") = fid;
	register long a7 __asm__("a7") = eid;

	/* When this is called, the CPU's execution mode switches from kernel mode (S-Mode) to OpenSBI mode (M-Mode), and OpenSBI's processing handler is invoked. Once it's done, it switches back to kernel mode, and execution resumes after the ecall instruction. */
	__asm__ __volatile__(
		"ecall"
		: "=r" (a0), "=r" (a1)
		: "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7) // specific eid calls specific functions
		: "memory"
	);

	return (struct sbiret) { .error = a0, .value = a1 };
}

void putchar(char ch) {
	sbi_call(ch, 0, 0, 0, 0, 0, 0, 1);
}

/*void* memset(void* buf, char c, size_t n) {*/
/*	uint8_t* p = (uint8_t*) buf;*/
/*	while (n--) *p++ = c;*/
/*	return buf;*/
/*}*/

void kernel_main(void) {
	// initialise bss to 0
	/*memset(__bss, 0, (size_t) (__bss_end - __bss));*/

	printf("Hello, World\n");
	printf("%d + %d = %x", 1, 2, 0x1234abcd);

	for (;;) {
		__asm__ __volatile__("wfi");
	}
}

__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void) {
	__asm__ __volatile__(
		"mv sp, %[stack_top]\n" // set stack ptr
		"j kernel_main\n" // jump kernel_main
		:
		: [stack_top] "r" (__stack_top) // pass stack top addr as %[stack_top]
	);
}
