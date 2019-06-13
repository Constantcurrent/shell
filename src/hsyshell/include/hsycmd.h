#ifndef _HSYCMD_H
#define _HSYCMD_H
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pwd.h>
#include "tools.h"
#ifndef PATHMAX
#define PATHMAX 1024
#endif
#ifndef BUF_SZ
#define BUF_SZ 64
#endif

int hsy_num_cmds(void);
void getUsername(char*);
void getHostname(char*);
int getCurWorkDir(char*);
int hsy_cd(char**,int);
int hsy_help(char**,int);
int hsy_exit(char**,int);
int hsy_bg(char**,int);
int hsy_ls(char**,int);
int hsy_launch(char**,int);
int hsy_execute(char**,int argc);
int ls_option(char**,int,int);
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
#endif
