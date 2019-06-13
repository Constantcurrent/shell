#include "hsycmd.h"

char *hsy_str[] = {
  "hsy_cd",
  "hsy_help",
  "hsy_exit",
  "hsy_bg",
  "hsy_ls"
};
int (*hsy_func[]) (char **,int) = {
  &hsy_cd,
  &hsy_help,
  &hsy_exit,
  &hsy_bg,
  &hsy_ls
};
void getUsername(char* uname) { // 获取当前登录的用户名
	struct passwd* pwd = getpwuid(getuid());
	strcpy(uname, pwd->pw_name);
}

void getHostname(char* hname) { // 获取主机名
	gethostname(hname, BUF_SZ);
}

int getCurWorkDir(char* path) { // 获取当前的工作目录
	char* result = getcwd(path, BUF_SZ);
	if (result == NULL)
		return 1;
	else return 0;
}

int hsy_num_cmds() {
  return (sizeof(hsy_str) / sizeof(char *));
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
    printf("  %s\n", hsy_str[i]);
  }
  printf("Use the man command for information on other programs.\n");
  return 1;
}

int hsy_bg(char **args,int argc)

{
   strcpy(args[0],"bg");
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

int ls_option(char** argv,int index,int ls_mode){
        char tmp_path[PATHMAX];
        char cur_path[PATHMAX];
        getCurWorkDir(cur_path);
        printf("test enter %s\n",cur_path);
        strcpy(tmp_path,cur_path);
        printf("path length is %s\n",tmp_path);
        if(argv[index]==NULL) dopath(cur_path,ls_mode);
        else{
          if(argv[index][0]=='/')   dopath(argv[index],ls_mode);
          else{
            strcat(tmp_path,argv[index]);
            dopath(tmp_path,ls_mode);
          }
        }
        return 1;
 
}
int hsy_ls(char **argv, int si){
         // char argc=char(si+'0');
          if(si==1)
             return ls_option(argv,0,1);
          if(argv[1][0]=='-'){
                char* c=argv[1];
		c++;
                strcat(c,"\0");
                printf("option char: %s\n", c);
                switch(*c++){
                case 'a':
                        if(*c=='l'){
                          printf("test opt");
                          ls_option(argv,2,3);
 			}
                        else
                          ls_option(argv,2,1);
                        //printf("optimization flag is open.\n\n");
                        break;
                case 'l':
                        if(*c=='a')
                          ls_option(argv,2,3);
                        else
                          ls_option(argv,2,2);
                        //printf("the obj is: %s\n\n", optarg);
                        break;   
                default:
                        fprintf(stderr, "miss option char in optstring.\n");
                                      }
        }else
             ls_option(argv,1,2);
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
    if (strcmp(args[0], hsy_str[i]) == 0) {
      return (*hsy_func[i])(args,argc);
    }
  }
  return hsy_launch(args,argc);
}

/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */


