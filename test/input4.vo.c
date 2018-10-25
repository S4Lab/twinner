#include <stdio.h>
#include <string.h>
#include <string.h> 
const char *str0 = "Usage: program xyzuw\n";
const char *str1 = "\tx=R|L first diamond\n";
const char *str2 = "\t\tyz=RR > u=R|L the RR diamond\n";
const char *str3 = "\t\tyz=RL > RL scenario\n";
const char *str4 = "\t\ty=L > z=R|L the L diamond\n";
const char *str5 = "\t\t\tw=R|L the last diamond\n";
const char *str6 = "right-path first diamond\n";
const char *str7 = "left-path first diamond\n";
const char *str8 = "right-path in the RR diamond\n";
const char *str9 = "left-path in the RR diamond\n";
const char *str10 = "the RL scenario\n";
const char *str11 = "right-path in the L diamond\n";
const char *str12 = "left-path in the L diamond\n";
const char *str13 = "right-path last diamond\n";
const char *str14 = "left-path last diamond\n";
const char program_text[] = {('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x01), (0x06), (0x06), (0x01), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x43'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x05), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\x85'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\x65'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x05), (0x07), ('\xBE'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x52'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x05), (0x07), ('\xAD'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x52'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x9E'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\xFD'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\xDD'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x05), (0x07), ('\xBE'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x52'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x37'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\xC7'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x52'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x51'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\x5A'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\x5F'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x05), (0x07), ('\x40'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x52'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\x0E'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x05), (0x07), ('\xDE'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x52'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00')};
char program[sizeof(program_text)];

const char *init_program(int *argcptr,char *argv[])
{
  memcpy(program,program_text,sizeof(program_text));
   *((void **)(&program[0])) = argv;
   *((int **)(&program[12])) = argcptr;
   *((const char **)(&program[37])) = str0;
   *((const char **)(&program[47])) = str1;
   *((const char **)(&program[57])) = str2;
   *((const char **)(&program[67])) = str3;
   *((const char **)(&program[77])) = str4;
   *((const char **)(&program[87])) = str5;
   *((const char **)(&program[191])) = str6;
   *((const char **)(&program[207])) = str7;
   *((const char **)(&program[350])) = str8;
   *((const char **)(&program[366])) = str9;
   *((const char **)(&program[382])) = str10;
   *((const char **)(&program[453])) = str11;
   *((const char **)(&program[469])) = str12;
   *((const char **)(&program[534])) = str13;
   *((const char **)(&program[550])) = str14;
  return program;
}
