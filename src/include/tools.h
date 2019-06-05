#include <sys/types.h>          /* some systems still require this */
#include <sys/stat.h>
#include <time.h>
#include <sys/termios.h>        /* for winsize */
#include <sys/ioctl.h>
#include <stdio.h>              /* for convenience */
#include <stdlib.h>             /* for convenience */
#include <stddef.h>             /* for offsetof */
#include <string.h>             /* for convenience */
#include <unistd.h>             /* for convenience */
#include <signal.h>
#include <stdbool.h>
#ifndef _TOOLS_H
#define _TOOLS_H
#define	MAXLINE	4096			
#define HSY_STD_BUFSIZE 4096
// err funcion declerations
void    err_msg(const char *, ...);                     /* {App misc_source} */
void    err_dump(const char *, ...) __attribute__((noreturn));
void    err_quit(const char *, ...) __attribute__((noreturn));
void    err_cont(int, const char *, ...);
void    err_exit(int, const char *, ...) __attribute__((noreturn));
void    err_ret(const char *, ...);
void    err_sys(const char *, ...) __attribute__((noreturn));
char* path_alloc(size_t*);
//------------------lsdir----------------------
#include <stdbool.h> 
#include <dirent.h>
#include <limits.h>
typedef int  Myfunc(const char *, const struct stat *, int);
Myfunc   myfunc;
int      lsdir(char *, char ch);
int      dopath(char);
//static long     nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;
/** Descend through the hierarchy, starting at "pathname".
 * The caller's func() is called for every file.
 */
/** Descend through the hierarchy, starting at "pathname".
 * The caller's func() is called for every file.
 */
#define FTW_F   1               /* file other than directory */
#define FTW_D   2               /* directory */
#define FTW_DNR 3               /* directory that can't be read */
#define FTW_NS  4
#define LS_A    1
#define LS_L    2
#define LS_AL   3               /* file that we can't stat */
char   *fullpath;
size_t pathlen;          /* contains full pathname for every file */
#endif
