#include <stdio.h>
#include <string.h>
#include <string.h> 
const char *str0 = "multiplied";
const char *str1 = "UL\n";
const char *str2 = "UR\n";
const char *str3 = "DL\n";
const char *str4 = "DR\n";
const char *str5 = "sequential: U\n";
const char *str6 = "sequential: D\n";
const char *str7 = "sequential: L\n";
const char *str8 = "sequential: R\n";
const char *str9 = "sequential: F\n";
const char *str10 = "sequential: B\n";
const char *str11 = "slow";
const char *str12 = "slow\n";
const char *str13 = "fast\n";
const char program_text[] = {('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\xE3'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\xC3'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\xA3'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x05), (0x10), (0x0C), (0x09), ('\x8E'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x85'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\x77'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x55'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x37'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\x80'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x4C'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x31'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\x49'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x4C'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x93'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\xF2'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x55'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x05), (0x07), ('\xE1'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x4C'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x05), (0x07), ('\xD0'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x46'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x05), (0x10), (0x0C), (0x09), ('\x4F'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00')};
char program[sizeof(program_text)];

const char *init_program(int *argcptr,char *argv[])
{
  memcpy(program,program_text,sizeof(program_text));
   *((void **)(&program[0])) = argv;
   *((const char **)(&program[120])) = str0;
   *((const char **)(&program[191])) = str1;
   *((const char **)(&program[207])) = str2;
   *((const char **)(&program[246])) = str3;
   *((const char **)(&program[262])) = str4;
   *((const char **)(&program[301])) = str5;
   *((const char **)(&program[317])) = str6;
   *((const char **)(&program[350])) = str7;
   *((const char **)(&program[366])) = str8;
   *((const char **)(&program[399])) = str9;
   *((const char **)(&program[415])) = str10;
   *((const char **)(&program[439])) = str11;
   *((const char **)(&program[464])) = str12;
   *((const char **)(&program[480])) = str13;
  return program;
}
