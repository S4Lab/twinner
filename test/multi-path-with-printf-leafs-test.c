#include <stdio.h>

int main (int argc, char *argv[]) {
	if (argc < 2) {
		printf ("Usage: program <command codes>\n");
		return -1;
	}
	int a = argv[1][0] - '0';
	int b = argv[1][1] - '0';
	int c = argv[1][2] - '0';
	if (a > 5) {
		if (b > 5) {
			if (c > 5) {
				printf ("a > 5, b > 5, c > 5 -- case 1\n");
			} else {
				printf ("a > 5, b > 5, c <= 5 -- case 2\n");
			}
		} else if (c > 5) {
			printf ("a > 5, b <= 5, c > 5 -- case 3\n");
		} else {
			printf ("a > 5, b <= 5, c <= 5 -- case 4\n");
		}
	} else if (b > 5) {
		if (c > 5) {
			printf ("a <= 5, b > 5, c > 5 -- case 5\n");
		} else {
			printf ("a <= 5, b > 5, c <= 5 -- case 6\n");
		}
	} else {
		if (c > 5) {
			printf ("a <= 5, b <= 5, c > 5 -- case 7\n");
		} else {
			printf ("a <= 5, b <= 5, c <= 5 -- case 8\n");
		}
	}
	printf ("multi-path program with printf-leafs\n");
	return 0;
}

