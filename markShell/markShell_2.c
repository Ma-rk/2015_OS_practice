#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

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
int copy_file(char **argVector);
int remove_file(char **argVector);
int move_file(char **argVector);
int change_directory(char **argVector);
int print_working_directory(void);
int make_directory(char **argVector);
int remove_directory(char **argVector);
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
      if (argCount > 3) { // too many args
        printf("usage of ls commend: ls [path]\n");
      } else if ((argCount <= 2) || strcmp(argVector[1], ">") == 0){
        executionResult = list_files(argCount, argVector);
      } else {

      }
      continue;
    }

    /*
      cp
    */
    if (!strcmp(argVector[0], "cp")){
      if (argCount != 3) { // number of args doesn't match
        printf("usage of cp commend: cp [source file] [destination file]\n");
      } else {
        executionResult = copy_file(argVector);
        printf("result of cp: %d\n", executionResult);
      }
      continue;
    }

    /*
      rm
    */
    if (!strcmp(argVector[0], "rm")){
      if (argCount != 2) { // number of args doesn't match
        printf("usage of rm commend: rm [destination file]\n");
      } else {
        executionResult = remove_file(argVector);
        printf("result of rm: %d\n", executionResult);
      }
      continue;
    }

    /*
      mv
    */
    if (!strcmp(argVector[0], "mv")){
      if (argCount != 3) { // number of args doesn't match
        printf("usage of rm commend: mv [source file] [destination file]\n");
      } else {
        executionResult = move_file(argVector);
        printf("result of mv: %d\n", executionResult);
      }
      continue;
    }

    /*
      mv
    */
    if (!strcmp(argVector[0], "cd")){
      if (argCount != 2) { // number of args doesn't match
        printf("usage of cd commend: cd [destination directory]\n");
      } else {
        executionResult = change_directory(argVector);
        printf("result of cd %d\n", executionResult);
      }
      continue;
    }

    /*
      pwd
    */
    if (!strcmp(argVector[0], "pwd")){
      if (argCount != 1) { // number of args doesn't match
        printf("usage of pwd commend: pwd\n");
      } else {
        executionResult = print_working_directory();
        printf("result of pwd: %d\n", executionResult);
      }
      continue;
    }

    /*
      mkdir
    */
    if (!strcmp(argVector[0], "mkdir")){
      if (argCount != 2) { // number of args doesn't match
        printf("usage of mkdir commend: mkdir [new directory name]\n");
      } else {
        executionResult = make_directory(argVector);
        printf("result of mkdir: %d\n", executionResult);
      }
      continue;
    }

    /*
      rmdir
    */
    if (!strcmp(argVector[0], "rmdir")){
      if (argCount != 2) { // number of args doesn't match
        printf("usage of rmdir commend: rmdir [existing directory name]\n");
      } else {
        executionResult = remove_directory(argVector);
        printf("result of rmdir: %d\n", executionResult);
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
  check the existence of file or directory
*/
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

/*
  list files
*/
int list_files(int argCount, char **argVector)
{
  printf("===>executing ls...\n");
  DIR *dp;
  struct dirent *d_entry;
  char pathArg[MAX_ARG_LENGTH] = ".";
  int intFileOrDir, fd, temp_stdout;

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

  if(argCount == 3){
    fd = creat(argVector[2], 0644);
    temp_stdout = dup(1);
    dup2(fd, 1);
    close(fd);
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

  dup2(temp_stdout, 1);

  return 0;
}

/*
  copy_file
*/
int copy_file(char **argVector)
{
  int arg1 = isFileOrDir(argVector[1]);
  int arg2 = isFileOrDir(argVector[2]);

  if(arg1 != ARG_FILE)
  {
    perror("First argument must be a existing file. Copy not executed.\n");
    return 1;
  }

  if(arg2 == ARG_FILE || arg2 == ARG_DIR)
  {
    perror("There is aleady a file or directory named argVector[1]. Copy not executed.\n");
    return 1;
  }

  int in_fd, out_fd, rd_count;
  char buffer[4096];
  if ((in_fd = open(argVector[1], O_RDONLY)) < 0)
    return 1;
  if ((out_fd = creat(argVector[2], 0644)) < 0)
    return 1;
  while (1) {
    if ((rd_count = read(in_fd, buffer, 4096)) <= 0)
      break;
    if (write(out_fd, buffer, rd_count) <= 0)
      return 1;
  }
  if (rd_count < 0)
    return 1;
    close(in_fd);
    close(out_fd);
  printf("end of cp...\n");
  return 0;
}

/*
  remove file
*/
int remove_file(char **argVector)
{
  if (isFileOrDir(argVector[1]) != ARG_FILE)
  {
    perror("First argument must be a existing file. Remove not executed.\n");
    return 1;
  }

  int rmResult = remove(argVector[1]);
 
  if( rmResult == 0 )
  {
    printf("rm succeed!!\n" );
  }
  else if( rmResult == -1 )
  {
    perror("rm failed...\n");
    return 1;
  }
  return 0;
}

/*
  move file
*/
int move_file(char **argVector)
{
  if(copy_file(argVector) != 0)
  {
    perror("mv error: copy of source file failed...\n");
    return 1;
  }
  strcpy(argVector[2], argVector[1]);
  if(remove_file(argVector) != 0)
  {
    perror("mv error: remove of source file failed...\n");
    return 1;
  }
  return 0;
}

/*
  change_directory
*/
int change_directory(char **argVector)
{
  if (isFileOrDir(argVector[1]) != ARG_DIR)
  {
    perror("cd error: wrong path...\n");
    return 1;
  }
  int cdResult = chdir(argVector[1]);

  if( cdResult == 0 )
  {
    print_working_directory();
  }
  else if( cdResult == -1 )
  {
    perror("failed to change directory...\n");
  }
  return 0;
}

/*
  print_working_directory
*/
int print_working_directory(void)
{
  char pwd[255];
  getcwd(pwd, 255);
  printf("currently working on [%s]\n", pwd);
  return 0;
}

/*
  make_directory
*/
int make_directory(char **argVector)
{
  int checkArg = isFileOrDir(argVector[1]);
  if (checkArg == ARG_DIR || checkArg == ARG_FILE)
  {
    perror("mkdir error: There is aleady same name of file or directory...\n");
    return 1;
  }

  int mkdirResult = mkdir(argVector[1], 0755);
  if(mkdirResult == 0)
  {
    printf("mkdir succeed.\n");
  }
  else if(mkdirResult == -1)
  {
    perror("mkdir failed...\n");
  }
  return 0;
}

/*
  remove_directory
*/
int remove_directory(char **argVector)
{
  int checkArg = isFileOrDir(argVector[1]);
  if (checkArg != ARG_DIR)
  {
    perror("rmdir error: There is no such a directory...\n");
    return 1;
  }
  int rmdirResult = rmdir(argVector[1]);

  if( rmdirResult == 0 )
  {
      printf("rmdir succeed.\n");
  }
  else if( rmdirResult == -1 )
  {
      perror("rmdir failed...\n" );
  }
  return 0;
}