#include <stdio.h>

int main () {
	volatile long long int tt = 0x0102030405060708;
	const int dd = 0xA1A2A3A4;
	const int ee = 0xB1B2;
	const int ff = 0xC1;
	asm (
			"movq %0, %%rax\n\t"
			"movl %1, %%eax\n\t"
			"movw %2, %%ax\n\t"
			"movb %3, %%ah\n\t"
			"movq %%rax, %0\n\t"
			: "+b" (tt) : "m" (dd), "m" (ee), "m" (ff) : "memory"
	    );
	printf ("Testing with RAX => 0x%llX\n", tt);
	tt = 0x0102030405060708;
	asm (
			"movq %0, %%r12\n\t"
			"movl %1, %%r12d\n\t"
			"movw %2, %%r12w\n\t"
			"movb %3, %%r12b\n\t"
			"movq %%r12, %0\n\t"
			: "+b" (tt) : "m" (dd), "m" (ee), "m" (ff) : "memory"
	    );
	printf ("Testing with R12 => 0x%llX\n", tt);
	tt = 0x0102030405060708;
	asm (
			"movq %0, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %%rdi, %0\n\t"
			: "+b" (tt) : "m" (dd), "m" (ee), "m" (ff) : "memory"
	    );
	printf ("Testing with RDI => 0x%llX\n", tt);
	return 0;
}

