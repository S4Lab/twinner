#include <stdio.h>

int main () {
	volatile int out = 50, t1, t2;
	asm (
			"mov $0x10, %1\n\t"
			"mov $0x11, %2\n\t"
			"sub %1, %2\n\t"
			"mov %2, %0\n\t"
			: "=r" (out)
      : "r" (t1), "r" (t2)
	    );
	printf ("Result: %d\n", out);
	return 0;
}

