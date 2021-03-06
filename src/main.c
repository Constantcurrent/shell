#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "tools.h"
#include "main.h"
//int getopt(int argc, char * const argv[],const char *optstring);
extern char *optarg;
char* envir;
extern int optind, opterr, optopt;
bool IS_REDIRECTION=false;
int left=0;
int hsy_num_cmds() {
  return (sizeof(cmds_str) / sizeof(char *));
}

int hsy_cd(char **args,int argc)
{ 
  //printf("%s\n",args[1]);
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
/** dir=path_alloc(ptr,&size);
     getcwd(ptr,size)
*/
  }
  is_cd=true;
  return 1;
}

/**
   @brief Builtin command: print help.
   @ram args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int hsy_help(char **args,int argc)
{
  int i;
  printf("welcome to the shell of hsy,\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");
  for (i = 0; i < hsy_num_cmds(); i++) {
    printf("  %s\n", cmds_str[i]);
  }
  printf("Use the man command for information on other programs.\n");
  return 1;
}

int hsy_bg(char **args,int argc)
{
   return hsy_launch(args,argc);
}
/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int hsy_exit(char **args,int argc)
{
  return 0;
}

int ls_option(char** argv,int len,int index,int ls_mode){
        size_t tmp_size;
        char* tmp_path;
        tmp_path=path_alloc(&tmp_size);
        strcpy(tmp_path,cur_path);
        strcat(tmp_path,"/");
        if(index+1==len||argv[index][0]=='>'||argv[index][0]=='<'||argv[index][0]) lsdir(cur_path,ls_mode);
        else{
          if(argv[index][0]=='/')    lsdir(argv[index],ls_mode);
          else{
            strcat(tmp_path,argv[index]);
            lsdir(tmp_path,ls_mode);
          }
        }
        free(tmp_path);           /* does it all */
        return 1;
 
}
int hsy_ls(char **argv, int si){
          char c;
         // char argc=char(si+'0');
          int index=1;
          if(si==1)
             return ls_option(argv,si,0,1);
          char *ch=argv[1];
          if(*ch++!='-')
             return ls_option(argv,si,1,1);
          if((c=*ch++)){
                index++;
                printf("option char: %c\n", c);
                switch(c){
                case 'a':
                        if(*ch=='l'){
                          printf("test opt");
                          ls_option(argv,si,index,1);
 			}
                        else
                          ls_option(argv,si,index,3);
                        //printf("optimization flag is open.\n\n");
                        break;
                case 'l':
                        if(*ch=='a')
                          ls_option(argv,si,index,2);
                        else
                          ls_option(argv,si,index,3);
                        //printf("the obj is: %s\n\n", optarg);
                        break;   
                default:
                        fprintf(stderr, "miss option char in optstring.\n");
                                      }
        }
        if(index==0&&c==-1){
             ls_option(argv,si,index,1);
        }
        return 1;				
}
/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int hsy_launch(char **args,int argc)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/**:
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int hsy_execute(char **args,int argc)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < hsy_num_cmds(); i++) {
    if (strcmp(args[0], cmds_str[i]) == 0) {
      return (*cmds_func[i])(args,argc);
    }
  }
  return hsy_launch(args,argc);
}

/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */

char **hsy_split_line(char *line,int* argc)
{
  int bufsize = HSY_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, HSY_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    if(token[0]==COM_REDI_OUT||token[0]==COM_REDI_IN){
      IS_REDIRECTION=true;
      left=position;
    }
    position++;

    if (position >= bufsize) {
      bufsize += HSY_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        free(tokens_backup);
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, HSY_TOK_DELIM);
  }
  tokens[position] = NULL;
  *argc=position;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void hsy_loop(void)
{
  char *line;
  char **args;
  int status;
  int argc=0;
  cur_path=path_alloc(&size);
  do {
    IS_REDIRECTION=false;
    if(is_cd){
       if (getcwd(cur_path,size) == NULL)
                err_sys("getcwd failed");
       is_cd=false;
    }
    printf("%s %s@hsy: ",envir,cur_path);
    line = hsy_read_line();
//    printf("%s\n",line);
    free(line);
    free(args);
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.
  envir=getenv("TEST");
  // Run command loop.
  hsy_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

