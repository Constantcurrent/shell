#include "tools.h"

int dopath(char* pathname, int ls_mode)
{
	struct stat statbuf;
	struct dirent *dirp;
	DIR *dp;
	int ret=1, n;
        bool is_al=false;
	if (lstat(pathname, &statbuf) < 0)	/* stat error */
		return(myfunc(pathname, &statbuf, FTW_NS));
	if (S_ISDIR(statbuf.st_mode) == 0)	/* not a directory */
		return(myfunc(pathname, &statbuf, FTW_F));
	/*
	 * It's a directory.  First call func() for the directory,
	 * then process each filename in the directory.
	 */
	if ((ret = myfunc(pathname, &statbuf, FTW_D)) != 0)
		return(ret);
	n = strlen(pathname);
        if(pathname[n-1]!='/')
	  pathname[n++] = '/';
	pathname[n] = 0;
         if ((dp = opendir(pathname)) == NULL)   /* can't read directory */
                return(myfunc(pathname, &statbuf, FTW_DNR));
        char tmp_path[PATHMAX]; // directory entry path
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
                       strcpy(tmp_path,pathname);
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
                err_ret("can't close directory %s", pathname);
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
