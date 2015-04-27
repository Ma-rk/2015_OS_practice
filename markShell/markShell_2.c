#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
/* constants  */
#define MAX_CMD_LENGTH 1024
#define MAX_ARG_LENGTH 128 
#define ARG_FILE   0
#define ARG_DIR    1
#define ARG_WRONG -1

/* global variables */
char prt[] = "MarkSh> ";

/* function declarateion */
void parse_cmdLine(char * cmdLine, int * argCount, char ** argVector);
void print_cmd_argv(int argCount, char ** argVector);
int isFileOrDir(char* path);

int list_files(int argCount, char**argVector);

/* 
main 
*/
int main(void)
{
  char * argVector[MAX_ARG_LENGTH];
  int argCount;
  char cmdLine[MAX_CMD_LENGTH];

  while(1)
  {
    int executionResult = 0;

    // display prompt
    printf("%s", prt);
    fflush(stdout);

    // read input
    fgets(cmdLine, MAX_CMD_LENGTH, stdin);
    cmdLine[strlen(cmdLine) - 1] = '\0';

    parse_cmdLine(cmdLine, &argCount, argVector);

    /*
      exit
    */
    if (!strcmp(argVector[0], "exit")){
      printf("Quit MarkShell...\n");
      return 0;
    }

    /*
      ls
    */
    if (!strcmp(argVector[0], "ls")){
      if (argCount > 2) {
        // too many args
        printf("usage of ls commend: ls [path]\n");
      } else {
        if((executionResult = list_files(argCount, argVector)) != 0)
          return 1;
      }
      continue;
    }

    printf("%s\n", "wrong command...");
  }
  
  return 0;
}

/* 
  parse user input string 
*/
void parse_cmdLine(char *cmdLine, int *argCount, char **argVector)
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

/*
  list files
*/
int list_files(int argCount, char **argVector)
{
  printf("===>executing ls...\n");
  DIR *dp;
  struct dirent *d_entry;
  char pathArg[MAX_ARG_LENGTH] = ".";
  int intFileOrDir;
  if (argCount == 2)
  {
    intFileOrDir = isFileOrDir(argVector[1]);
    if (intFileOrDir == ARG_FILE)
    { 
      printf("%s is a file. \n", argVector[1]);
      return 0;
    }
    else if (intFileOrDir == ARG_DIR)
    {
      strcpy(pathArg, argVector[1]);
    }
    else if (intFileOrDir == ARG_WRONG)
    {
      printf("%s\n", "wrong path");
      return 0;
    }
  }

  dp = opendir(pathArg);
  if (dp == NULL)
  {
    perror("directory open error\n");
    return 1;
  }
  d_entry = readdir(dp);

  while (d_entry != NULL)
  {
    printf("%s\n", d_entry->d_name);
    d_entry = readdir(dp);
  }
  closedir(dp);
  return 0;
}

int isFileOrDir(char* path)
{
  struct stat buf;
  int isFile;
  if(stat(path, &buf) == 0)
  {
    if(buf.st_mode & S_IFDIR)
    {
       isFile = ARG_DIR;
    }
    else
    {
       isFile = ARG_FILE;
    }
  }
  else
  {
       isFile = ARG_WRONG;
  }
  return isFile;
}