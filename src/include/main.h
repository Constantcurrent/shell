#ifndef _MAIN_H
#define _MAIN_H
#define HSY_TOK_BUFSIZE 64
#define HSY_TOK_DELIM " \t\r\n\a"
#define HSY_RL_BUFSIZE 1024
int hsy_cd(char**,int);
int hsy_help(char**,int);
int hsy_exit(char**,int);
int hsy_bg(char**,int);
int hsy_ls(char**,int);
int hsy_launch(char**,int);
char **hsy_split_line(char*,int* );
char *hsy_read_line(void);
bool is_cd=true;
char* cur_path;
size_t size;
/*
  List of builtin commands, followed by their corresponding functions.
 */
/*

int hsy_cd(char **args);
int hsy_help(char **args);
int hsy_exit(char **args);
int hsy_bg(char **args);
int hsy_launch(char **args);
bool is_cd=true;i
*/
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *cmds_str[] = {
  "hsy_cd",
  "hsy_help",
  "hsy_exit",
  "hsy_bg",
  "hsy_ls",
  "hsy_ll"
  
};
int ls_option(char**,int,int,int);
int (*cmds_func[]) (char **,int) = {
  &hsy_cd,
  &hsy_help,
  &hsy_exit,
  &hsy_bg,
  &hsy_ls
};

#endif
