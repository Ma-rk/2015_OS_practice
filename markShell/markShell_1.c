#include <stdio.h>

/* constants  */
#define MAX_CMD_LENGTH 1024

/* global variables */
char prt[] = "MarkSh> ";

int main(void)
{
  char cmdLine[MAX_CMD_LENGTH];

  while(1)
  {

    // display prompt
    printf("%s", prt);
    fflush(stdout);

    // read input
    fgets(cmdLine, MAX_CMD_LENGTH, stdin);
    printf("%s\n", cmdLine);
  }
  
  return 0;
}
