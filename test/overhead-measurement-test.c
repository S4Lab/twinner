#include <stdio.h>

int main (int argc, char *argv[]) {
  const int n = 10;
  int a = 1;
  int b = 1;
  int i;
  for (i = 0; i < n; ++i) {
    int c = a + b;
    a = b;
    b = c;
  }
  printf ("The %d-th fibonacci number is %d.\n", n, a);
	return 0;
}

