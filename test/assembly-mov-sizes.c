#include <stdio.h>

int main () {
	volatile long long int tt = 0x0102030405060708;
	int dd = 0xA1A2A3A4;
	int ee = 0xB1B2;
	int ff = 0xC1;
	asm (
			"movq %0, %%rax\n\t"
			"movl %1, %%eax\n\t"
			"movw %2, %%ax\n\t"
			"movb %3, %%ah\n\t"
			"movq %%rax, %0\n\t"
			: "+b" (tt) : "m" (dd), "m" (ee), "m" (ff) : "memory"
	    );
	printf ("0x%llX\n", tt);
	return 0;
}

