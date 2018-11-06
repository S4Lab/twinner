#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (argc <= 3) {
    printf ("At least three arguments are required\n");
    return -1;
  }
  if (argc > 6) {
    printf ("At most five arguments are allowed\n");
    return -2;
  }
  if (strcmp (argv[1], "magicvalue") != 0) {
    printf ("You are not allowed to run this program!\n");
    return -3;
  }
  const char a = argv[2][0];
  const char b = argv[2][1];
  if (a == 'R') {
    if (strcmp (argv[3], "left")) {
      printf ("left-path of R scenario\n");
    } else {
      printf ("right-path of R scenario\n");
    }
  } else if (b == 'R') {
    printf ("LR scenario\n");
  } else {
    if (strcmp (argv[3], "left") == 0) {
      printf ("left-path of LL scenario\n");
    } else {
      printf ("right-path of LL scenario\n");
    }
    if (strcmp (argv[4], argv[5]) == 0) {
      printf ("fourth and fifth arguments are the same\n");
    } else {
      printf ("last args are different\n");
    }
  }
  return 0;
}

