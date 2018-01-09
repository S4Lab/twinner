#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (argc < 5) {
    printf ("Usage: program --left <num1> --right <num2>\n");
    return -1;
  }
  if (strcmp (argv[1], "--left") != 0) {
    printf ("left option is missing!\n");
    return -2;
  }
  if (argv[2][1] != 0 || argv[2][0] < '0' || argv[2][0] > '9') {
    printf ("invalid left value!\n");
    return -3;
  }
  const int num1 = argv[2][0] - '0';
  if (strcmp (argv[3], "--right") != 0) {
    printf ("right option is missing!\n");
    return -4;
  }
  if (argv[4][1] != 0 || argv[4][0] < '0' || argv[4][0] > '9') {
    printf ("invalid right value!\n");
    return -5;
  }
  const int num2 = argv[4][0] - '0';
  if (num1 < num2) {
    printf ("num1 is smaller than num2\n");
  } else {
    printf ("num1 is NOT smaller than num2\n");
  }
  if (num1 + num2 > 15) {
    printf ("sum of two numbers is more than 15\n");
  } else {
    printf ("sum of two numbers is NOT more than 15\n");
  }
  printf ("last line!\n");
  return 0;
}

