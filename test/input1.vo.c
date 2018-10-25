#include <stdio.h>
#include <string.h> 
const char *str0 = "Usage: program <command codes>\n";
const char *str1 = "first condition; then part\n";
const char *str2 = "first condition; else part\n";
const char *str3 = "second condition; then part\n";
const char *str4 = "second condition; else part\n";
const char program_text[] = {('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x01), (0x06), (0x01), (0x01), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x11'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x05), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x05), (0x0D), (0x08), ('\xF5'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0E), (0x0A), (0x0C), (0x09), ('\xB3'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x30'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x05), (0x0D), (0x08), ('\xF5'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0E), (0x0A), (0x0C), (0x09), ('\x8A'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x30'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x05), (0x0D), (0x08), ('\xF5'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0E), (0x0A), (0x0C), (0x09), ('\x61'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x30'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x07), (0x06), (0x03), (0x08), ('\x81'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x08'), ('\x00'), ('\x00'), ('\x00'), (0x06), (0x02), (0x08), ('\x9E'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x03), (0x08), ('\x96'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x05'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00')};
char program[sizeof(program_text)];

const char *init_program(int *argcptr,char *argv[])
{
  memcpy(program,program_text,sizeof(program_text));
   *((void **)(&program[0])) = argv;
   *((int **)(&program[12])) = argcptr;
   *((const char **)(&program[37])) = str0;
   *((const char **)(&program[214])) = str1;
   *((const char **)(&program[230])) = str2;
   *((const char **)(&program[263])) = str3;
   *((const char **)(&program[279])) = str4;
  return program;
}
