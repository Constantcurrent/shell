#include "tools.h"
#include <errno.h>
#include <limits.h>

#ifdef  PATH_MAX
static long     pathmax = PATH_MAX;
#else
static long     pathmax = 0;
#endif
#define _XOPEN_SOURCE 700
static long     posix_version = 0;
static long     xsi_version = 0;

/* If PATH_MAX is indeterminate, no guarantee this is adequate */
#define PATH_MAX_GUESS  1024
char* path_alloc(size_t* sizep) /* also return allocated size, if nonnull */
{
        char    *ptr;
        size_t  size;

        if (posix_version == 0)
                posix_version = sysconf(_SC_VERSION);

        if (xsi_version == 0)
                xsi_version = sysconf(_SC_XOPEN_VERSION);

        if (pathmax == 0) {             /* first time through */
                errno = 0;
                if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
                        if (errno == 0)
                                pathmax = PATH_MAX_GUESS;       /* it's indeterminate */
                        else
                                err_sys("pathconf error for _PC_PATH_MAX");
                } else {
                        pathmax++;              /* add one since it's relative to root */
                }
        }
        if ((posix_version < 200112L) && (xsi_version < 4))
                size = pathmax + 1;
        else
                size = pathmax;

        if ((ptr = malloc(size)) == NULL)
                err_sys("malloc error for pathname");

        if (sizep != NULL)
                *sizep = size;
        return(ptr);
}
/**
   this is referenced for read the content of directory
   
*/
int lsdir(char *pathname, char ch)
{
        fullpath=path_alloc(&pathlen); 					/* ({Prog pathalloc}) */
	if (pathlen <= strlen(pathname)) {
		pathlen = strlen(pathname) * 2;
		if ((fullpath = realloc(fullpath, pathlen)) == NULL)
			err_sys("realloc failed");
	}
	strcpy(fullpath, pathname);
        printf("%s\n",fullpath);
	return(dopath(ch));
}

/*
 * Descend through the hierarchy, starting at "fullpath".
 * If "fullpath" is anything other than a directory, we lstat() it,
 * call func(), and return.  For a directory, we call ourself
 * recursively for each name in the directory.
 */
int dopath(char ls_mode)
{
	struct stat statbuf;
	struct dirent *dirp;
	DIR *dp;
	int ret=1, n;
        bool is_al=false;
	if (lstat(fullpath, &statbuf) < 0)	/* stat error */
		return(myfunc(fullpath, &statbuf, FTW_NS));
	if (S_ISDIR(statbuf.st_mode) == 0)	/* not a directory */
		return(myfunc(fullpath, &statbuf, FTW_F));

	/*
	 * It's a directory.  First call func() for the directory,
	 * then process each filename in the directory.
	 */
	if ((ret = myfunc(fullpath, &statbuf, FTW_D)) != 0)
		return(ret);
	n = strlen(fullpath);
	if (n + NAME_MAX + 2 > pathlen) { // expand path buffer 
		pathlen *= 2;
		if ((fullpath = realloc(fullpath, pathlen)) == NULL)
			err_sys("realloc failed");
	}
	fullpath[n++] = '/';
	fullpath[n] = 0;
         if ((dp = opendir(fullpath)) == NULL)   /* can't read directory */
                return(myfunc(fullpath, &statbuf, FTW_DNR));
        char* tmp_path=path_alloc(&pathlen); // directory entry path
        struct stat entry_stat;// directory entry stat structure
        while ((dirp = readdir(dp)) != NULL) {
             switch(ls_mode){
                  case LS_A:printf("%s\n",dirp->d_name);break;// -a option
                  case LS_AL:        //-al or -la option
                       is_al=1;
                       ls_mode=LS_L;
                  case LS_L:      // -l option
                       if(is_al==0&&dirp->d_name[0]=='.')
                            break;
                       else{
                           is_al=0;
                           ls_mode=LS_AL;
                       }
                       printf("%s  ",dirp->d_name);
                       strcpy(tmp_path,fullpath);
                       strcat(tmp_path,dirp->d_name);
                       lstat(tmp_path,&entry_stat);
                       // print the entry information
                       printf("%d  ",entry_stat.st_mode);
                       printf("%ld  ",entry_stat.st_ino);
                       printf("%ld  ",entry_stat.st_dev);
                       printf("%ld  ",entry_stat.st_nlink);
                       printf("%d  ",entry_stat.st_uid);
                       printf("%d  ",entry_stat.st_gid);
                       printf("%ld  ",entry_stat.st_size);
                       printf("%s  ",ctime(&entry_stat.st_atim.tv_sec));
                       printf("%s  ",ctime(&entry_stat.st_mtim.tv_sec));
                       printf("%s  ",ctime(&entry_stat.st_ctim.tv_sec));
                       printf("%ld  ",entry_stat.st_blksize);
                       printf("%ld  ",entry_stat.st_blocks);
                       
                       break;
                    default:
                       printf("cannot find your option: please check\nthe available option: -a, -l, -al, -la\n");
                       return 1;
             }
                //strcpy(&fullpath[n], dirp->d_name);   /* append name after "/" */
                //if ((ret = dopath(func)) != 0)                /* recursive */
        }
        //fullpath[n-1] = 0;    /* erase everything from slash onward */

        if (closedir(dp) < 0)
                err_ret("can't close directory %s", fullpath);
        return(ret);
}

int myfunc(const char *pathname, const struct stat *statptr, int type)
{
        switch (type) {
        case FTW_F:
                switch (statptr->st_mode & S_IFMT) {
                case S_IFREG:   printf("%d \n",type);           break;
                case S_IFBLK:   printf("%d \n",type);           break;
                case S_IFCHR:   printf("%d \n",type);           break;
                case S_IFIFO:           break;
                case S_IFLNK:           break;
                case S_IFSOCK:          break;
                case S_IFDIR:   /* directories should have type = FTW_D */
                        err_dump("for S_IFDIR for %s", pathname);
                }
	case FTW_D:
                break;

        case FTW_DNR:
                err_ret("can't read directory %s", pathname);
                break;
        case FTW_NS:
                err_ret("stat error for %s", pathname);
                break;
        default:
                err_dump("unknown type %d for pathname %s", type, pathname);
        }
        return(0);
}
