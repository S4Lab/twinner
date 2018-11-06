#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (argc != 4) {
    printf ("Three arguments are required\n");
    return -2;
  }
  if (strcmp (argv[1], "--lowercase") == 0) {
    if (argv[2][0] == 'a') {
      printf ("2 -> a\n");
    } else {
      printf ("2 !-> a\n");
    }
    if (argv[2][1] == 'b') {
      printf ("2 -> b\n");
    } else {
      printf ("2 !-> b\n");
    }
  } else {
    if (argv[2][0] == 'A') {
      printf ("2 -> A\n");
    } else {
      printf ("2 !-> A\n");
    }
    if (argv[2][1] == 'B') {
      printf ("2 -> B\n");
    } else {
      printf ("2 !-> B\n");
    }
  }
  if (strcmp (argv[3], "sealed") != 0) {
    printf ("unsealed arguments\n");
  } else {
    printf ("SEALED!\n");
  }
  return 0;
}

