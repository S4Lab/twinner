#include <stdio.h>

int main (int argc, char *argv[]) {
	int a = argv[1][0] - '0'; // == 3
	int b = argv[2][0] - '0'; // == 8
	int c = a*2+3; // == 9
	int d = b - 3; // == 5
	if (c == d + 4) {
		c = 0;
	} else {
		c = 32;
	}
	return 0;
}

