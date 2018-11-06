#include <stdio.h>
#include <string.h>
#include <string.h> 
const char *str0 = "Three arguments are required\n";
const char *str1 = "--lowercase";
const char *str2 = "2 -> a\n";
const char *str3 = "2 !-> a\n";
const char *str4 = "2 -> b\n";
const char *str5 = "2 !-> b\n";
const char *str6 = "2 -> A\n";
const char *str7 = "2 !-> A\n";
const char *str8 = "2 -> B\n";
const char *str9 = "2 !-> B\n";
const char *str10 = "sealed";
const char *str11 = "unsealed arguments\n";
const char *str12 = "SEALED!\n";
const char program_text[] = {('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x01), (0x06), (0x06), (0x01), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x11'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x05), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x10), (0x0C), (0x09), ('\xC2'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x80'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x0A), (0x0C), (0x09), ('\x9B'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x61'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x05), (0x0A), (0x0C), (0x09), ('\x5E'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x62'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x7A'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x0A), (0x0C), (0x09), ('\x1B'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x41'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x05), (0x0A), (0x0C), (0x09), ('\xDE'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x42'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x06), (0x10), (0x0C), (0x09), ('\xA1'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00')};
char program[sizeof(program_text)];

const char *init_program(int *argcptr,char *argv[])
{
  memcpy(program,program_text,sizeof(program_text));
   *((void **)(&program[0])) = argv;
   *((int **)(&program[12])) = argcptr;
   *((const char **)(&program[37])) = str0;
   *((const char **)(&program[68])) = str1;
   *((const char **)(&program[128])) = str2;
   *((const char **)(&program[144])) = str3;
   *((const char **)(&program[189])) = str4;
   *((const char **)(&program[205])) = str5;
   *((const char **)(&program[256])) = str6;
   *((const char **)(&program[272])) = str7;
   *((const char **)(&program[317])) = str8;
   *((const char **)(&program[333])) = str9;
   *((const char **)(&program[357])) = str10;
   *((const char **)(&program[382])) = str11;
   *((const char **)(&program[398])) = str12;
  return program;
}
