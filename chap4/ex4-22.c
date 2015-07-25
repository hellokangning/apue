#include "apue.h"
#include <dirent.h>
#include <limits.h>

//function type that is called for each filename
typedef int MY_FUNC(const char *, const struct stat *, int);

static MY_FUNC my_func;
static int my_ftw(char *, MY_FUNC *);
static int do_path(MY_FUNC *);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int main(int argc, char *argv[])
{
    int ret;
    if (argc != 2)
    {
        err_quit("usage: ftw <starting-pathname>");
    }

    ret = my_ftw(argv[1], my_func);
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink, nsock;
    if (0 == ntot)
    {
        ntot = 1;
    }
    printf("regular files = %7ld, %5.2f %%\n", nreg, nreg*100.0/ntot);
    printf("directories = %7ld, %5.2f %%\n", ndir,  ndir*100.0/ntot);
    printf("block special = %7ld, %5.2f %%\n", nblk, nblk*100.0/ntot);
    printf("char special = %7ld, %5.2f %%\n", nchr, nchr*100.0/ntot);
    printf("FIFOs = %7ld, %5.2f %%\n", nfifo, nfifo*100.0/ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink*100.0/ntot);
    printf("sockets = %7ld, %5.2f %%\n", nsock, nsock*100.0/ntot);

    return 0;
}

//descend through the hierachy starting at "pathname"
//the caller's func() is called for every file
#define FTW_F 1
#define FTW_D 2
#define FTW_DNR 3
#define FTW_NS 4

static char *fullpath;
static size_t pathlen;

static int my_ftw(char *pathname, MY_FUNC *func)
{
    fullpath = path_alloc(&pathlen);    //malloc PATH_MAX+1 bytes

    if (pathlen <= strlen(pathname))
    {
        pathlen = strlen(pathname);
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
        {
            err_sys("realloc failed");
        }
    }

    strcpy(fullpath, pathname);
        
    return (do_path(func));
}

//descend through the hierachy, starting at "fullpath"
//if "fullpath" is anything other than a directory, we lstat() it
//call func(), and return. For a directory, we call ourself recursively for each name in the directory

static int do_path(MY_FUNC *func)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret, n;

    if (lstat(fullpath, &statbuf) < 0) //stat error
    {
        return (func(fullpath, &statbuf, FTW_NS));
    }

    if (S_ISDIR(statbuf.st_mode) == 0) //not a directory
    {
        return (func(fullpath, &statbuf, FTW_F));
    }

    //it is a directory, first call func() for the directory
    //then process each filename in the directory
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
    {
        return ret;
    }

    if (n + NAME_MAX + 2 > pathlen) //expand path buffer
    {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
        {
            err_sys("realloc failed");
        }
    }

    fullpath[n++] = '/';
    fullpath[n] = 0;
   
   if ((dp = opendir(fullpath)) == NULL) //cannot read directory
   {
      return (func(fullpath, &statbuf, FTW_DNR));
   }

  while ((dirp = readdir(dp)) != NULL)
  {
     if (strcmp(dirp->d_name, ".") == 0 
            || strcmp(dirp->d_name, "..") == 0)
        continue;
    strcpy(&fullpath[n], dirp->d_name);
   if ((ret = dopath(func)) != 0)   break;
  }

  fullpath[n-1] = 0;
  if (closedir(dp) < 0)
  {
      err_ret("cannot close directory %s", fullpath);
  }
  
  return ret;
}

static int my_func(const char *pathname, const struct stat *statptr, int type)
{
    switch (type)
    {
        case FTW_F:
            switch(statptr->st_mode & S_IFMT)
            {
                case S_IFREG:   nreg++;     break;
                case S_IFBLK:   nblk++;     break;
                case S_IFCHR:   nchr++;     break;
                case S_IFIFO:   nfifo++;    break;
                case S_IFLNK:   nslink++;   break;
                case S_IFSOCK:  nsock++;    break;
                case S_IFDIR:   err_dump("for S_IFDIR for %s", pathname);
            }
            break;
        case FTW_D:
            ndir++;
            break;
        case FTW_DNR:
            err_ret("cannot read directory %s", pathname);
            break;
        case FTW_NS:
            err_ret("stat error for %s", pathname);
            break;
        default:
            err_dump("unknown type %d for pathname %s", type, pathname);
    }

    return 0;
}

 
      
    
