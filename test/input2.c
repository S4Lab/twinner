#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (argc < 3) {
    printf ("Usage: program <command codes>\n");
    return -1;
  }
  if (strcmp (argv[1], "--option") != 0) {
    printf ("unknown option!\n");
    return -2;
  }
  if (strcmp (argv[2], "optvalue") != 0) {
    printf ("invalid value!\n");
    return -2;
  }
  printf ("correct option/value pair is given!\n");
  return 0;
}

