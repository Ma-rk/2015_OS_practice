#include <stdio.h>
#include <string.h>

/* constants  */
#define MAX_CMD_LENGTH 1024
#define MAX_ARG_LENGTH 128 

/* global variables */
char prt[] = "MarkSh> ";

/* function declarateion */
void parse_cmdLine(char * cmdLine, int * argCount, char ** argVector);
void print_cmd_argv(int argCount, char ** argVector);

/* main */
int main(void)
{
  char * argVector[MAX_ARG_LENGTH];
  int argCount;
  char cmdLine[MAX_CMD_LENGTH];

  while(1)
  {
    // display prompt
    printf("%s", prt);
    fflush(stdout);

    // read input
    fgets(cmdLine, MAX_CMD_LENGTH, stdin);

    parse_cmdLine(cmdLine, &argCount, argVector);
    print_cmd_argv(argCount, argVector);
  }
  
  return 0;
}

/* parse user input string */
void parse_cmdLine(char * cmdLine, int * argCount, char ** argVector)
{
  int count = 0;
  char * argc = strtok(cmdLine, " ");
  while(argc != NULL)
  {
    argVector[count] = argc;
    argc = strtok(NULL, " ");
    count++;
  }
  * argCount = count;
}

/* display user input command and arguments  */
void print_cmd_argv(int argCount, char ** argVector)
{
  printf("= arg Count: %d\n", argCount);
  int i = 0;
  while(i < argCount) {
    printf("== argv[%d]: %s\n", i, argVector[i]);
    i++;
  }
}

