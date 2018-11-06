#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  int ret;
  if (argc != 4) {
    printf ("Usage: program --opt <val> <digit1><digit2>\n");
    ret = -1;
  } else {
    ret = 0;
    if (strcmp (argv[1], "--opt") == 0) {
      if (argv[2][0] == 'z') {
        printf ("--opt z is given\n");
      } else {
        printf ("unknown value is given\n");
      }
    } else {
      printf ("--opt is missing\n");
    }
    const int digit1 = argv[3][0] - '0';
    if (digit1 > 6) {
      printf ("digit1 is more than six\n");
    } else {
      printf ("digit1 is NOT more than six\n");
    }
  }
  if (ret != 0) {
    printf ("some error happened\n");
  } else {
    printf ("exiting normally\n");
  }
  return ret;
}

