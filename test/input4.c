#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (argc != 2) {
    printf ("Usage: program xyzuw\n");
    printf ("\tx=R|L first diamond\n");
    printf ("\t\tyz=RR > u=R|L the RR diamond\n");
    printf ("\t\tyz=RL > RL scenario\n");
    printf ("\t\ty=L > z=R|L the L diamond\n");
    printf ("\t\t\tw=R|L the last diamond\n");
    return -1;
  }
  const char x = argv[1][0];
  const char y = argv[1][1];
  if (x == 'R') {
    printf ("right-path first diamond\n");
  } else {
    printf ("left-path first diamond\n");
  }
  if (y == 'R') {
    const char z = argv[1][2];
    const char u = argv[1][3];
    if (z == 'R') {
      if (u == 'R') {
        printf ("right-path in the RR diamond\n");
      } else {
        printf ("left-path in the RR diamond\n");
      }
    } else {
      printf ("the RL scenario\n");
    }
  } else {
    const char z = argv[1][2];
    if (z == 'R') {
      printf ("right-path in the L diamond\n");
    } else {
      printf ("left-path in the L diamond\n");
    }
  }
  const char w = argv[1][4];
  if (w == 'R') {
    printf ("right-path last diamond\n");
  } else {
    printf ("left-path last diamond\n");
  }
  return 0;
}

