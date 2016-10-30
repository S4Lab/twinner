#include <stdio.h>

int main (int argc, char *argv[]) {
  if (argc < 2) {
    printf ("Usage: program <command codes>\n");
    return -1;
  }
  int a = argv[1][0] - '0';
  int b = argv[1][1] - '0';
  int c = argv[1][2] - '0';
  if (a > 8 && b <= 3) {
    printf ("first condition; then part\n");
  } else {
    printf ("first condition; else part\n");
  }
  if (c > 5) {
    printf ("second condition; then part\n");
  } else {
    printf ("second condition; else part\n");
  }
  return 0;
}

