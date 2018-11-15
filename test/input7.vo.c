#include <stdio.h>
#include <string.h>
#include <string.h> 
const char *str0 = "At least three arguments are required\n";
const char *str1 = "At most five arguments are allowed\n";
const char *str2 = "magicvalue";
const char *str3 = "You are not allowed to run this program!\n";
const char *str4 = "left";
const char *str5 = "left-path of R scenario\n";
const char *str6 = "right-path of R scenario\n";
const char *str7 = "LR scenario\n";
const char *str8 = "left";
const char *str9 = "left-path of LL scenario\n";
const char *str10 = "right-path of LL scenario\n";
const char *str11 = "fourth and fifth arguments are the same\n";
const char *str12 = "last args are different\n";
const char program_text[] = {('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x01), (0x06), (0x02), (0x01), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x11'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x05), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x03), (0x01), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x06'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x11'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x05), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x06), (0x10), (0x0C), (0x09), ('\x96'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x11'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x05), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\x53'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\x33'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x05), (0x07), ('\xBE'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x52'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x40'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x10), (0x0C), (0x09), ('\x09'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\xAB'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\x87'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x52'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x84'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x10), (0x0C), (0x09), ('\xA0'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x05), (0x10), (0x0C), (0x09), ('\x5F'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), (0x0C), (0x09), ('\x54'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x05'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00')};
char program[sizeof(program_text)];

const char *init_program(int *argcptr,char *argv[])
{
  memcpy(program,program_text,sizeof(program_text));
   *((void **)(&program[0])) = argv;
   *((int **)(&program[12])) = argcptr;
   *((const char **)(&program[37])) = str0;
   *((int **)(&program[56])) = argcptr;
   *((const char **)(&program[81])) = str1;
   *((const char **)(&program[112])) = str2;
   *((const char **)(&program[137])) = str3;
   *((const char **)(&program[253])) = str4;
   *((const char **)(&program[273])) = str5;
   *((const char **)(&program[289])) = str6;
   *((const char **)(&program[328])) = str7;
   *((const char **)(&program[358])) = str8;
   *((const char **)(&program[383])) = str9;
   *((const char **)(&program[399])) = str10;
   *((const char **)(&program[450])) = str11;
   *((const char **)(&program[466])) = str12;
  return program;
}
