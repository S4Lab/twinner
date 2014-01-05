#include <stdio.h>

int main () {
	long long int rax = 0xA1A2A3A4A5A6A7A8;
	long long int rsi = 0xB1B2B3B4B5B6B7B8;
	long long int rsp = 0xC1C2C3C4C5C6C7C8;
	long long int rbp = 0xD1D2D3D4D5D6D7D8;
	long long int r11 = 0xE1E2E3E4E5E6E7E8;

	const long long int newrax = 0xA1A2A3A4A5A6A7A8;
	const long long int newrsi = 0xB1B2B3B4B5B6B7B8;
	const long long int newrsp = 0xC1C2C3C4C5C6C7C8;
	const long long int newrbp = 0xD1D2D3D4D5D6D7D8;
	const long long int newr11 = 0xE1E2E3E4E5E6E7E8;
	void *pointerToRax = &rax;
	asm (
		"movq	%%rsi, -8(%%rax)\n\t"
		"movq	%%rsp, -16(%%rax)\n\t"
		"movq	%%rbp, -24(%%rax)\n\t"
		"movq	%%r11, -32(%%rax)\n\t"

		"addq	$8, %%rax\n\t"

		"movq	8(%%rax), %%rsi\n\t"
		"movq	16(%%rax), %%rsp\n\t"
		"movq	24(%%rax), %%rbp\n\t"
		"movq	32(%%rax), %%r11\n\t"

		"subq	$8, %%rax\n\t"

		"movq	-8(%%rax), %%rsi\n\t"
		"movq	-16(%%rax), %%rsp\n\t"
		"movq	-24(%%rax), %%rbp\n\t"
		"movq	-32(%%rax), %%r11\n\t"
		: "=m" (rsi), "=m" (rsp), "=m" (rbp), "=m" (r11)
	       	: "ao" (pointerToRax)
		:
	);
	printf ("Old values =>\n\t"
			"rsi: 0x%llX\n\t"
			"rsp: 0x%llX\n\t"
			"rbp: 0x%llX\n\t"
			"r11: 0x%llX\n", rsi, rsp, rbp, r11);
	printf ("New values =>\n\t"
			"rsi: 0x%llX\n\t"
			"rsp: 0x%llX\n\t"
			"rbp: 0x%llX\n\t"
			"r11: 0x%llX\n", newrsi, newrsp, newrbp, newr11);
	return 0;
}

