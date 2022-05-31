//调用opendir和readdir函数对指定目录进行遍历操作
//然后打印输出指定目录中各种类型的文件数目
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

typedef	int Myfunc(const char *, const struct stat *, int);   //定义一个函数
static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;
//各种类型的文件数目对应的变量
char *path_alloc(int* size);

int main(int argc, char *argv[])
{
  int ret;
  if (argc != 2)
  {
     printf("请输入正确的参数!\n");   //参数错误
     return 1;
  }
  ret = myftw(argv[1], myfunc);		/* does it all */
  ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
  //计算文件总量
  if (ntot == 0)     //如果目录中没有文件则将ntot设置为1以避免除数为0
  {
    ntot = 1;	
  }
  //以下一次打印各种类型文件的数据
  printf("普通文件 = %7ld, %5.2f %%\n", nreg, nreg*100.0/ntot);
  printf("目录文件 = %7ld, %5.2f %%\n", ndir,ndir*100.0/ntot);
  printf("块设备文件 = %7ld, %5.2f %%\n", nblk,nblk*100.0/ntot);
  printf("字设备文件 = %7ld, %5.2f %%\n", nchr, nchr*100.0/ntot);
  printf("FIFOs = %7ld, %5.2f %%\n", nfifo,nfifo*100.0/ntot);
  printf("符号链接文件 = %7ld, %5.2f %%\n", nslink, nslink*100.0/ntot);
  printf("套接字文件 = %7ld, %5.2f %%\n", nsock,nsock*100.0/ntot);
  return ret;
}
//路径缓冲区分配函数
char *path_alloc(int* size)
{
  char *p = NULL;
  if(!size)
  { 
    return NULL;
  }
  p = malloc(256);
  if(p)
  {
    *size = 256;
  }
  else
  {
    *size = 0;
  }
  return p;
}

#define	FTW_F	1		//
#define	FTW_D	2		//目录
#define	FTW_DNR	3		//不能读的目录
#define	FTW_NS	4		//不能获得状态的文件

static char	*fullpath;	//存放每个文件完整路径

static int myftw(char *pathname, Myfunc *func)
{
  int len;
  fullpath = path_alloc(&len);	//给路径缓冲区分配一个长度
  strncpy(fullpath, pathname, len);	//复制文件名称
  fullpath[len-1] = 0;			
  return(dopath(func));
}
//获得文件的状态
static int dopath(Myfunc* func)
{
  struct stat	statbuf;
  struct dirent	*dirp;
  DIR *dp;
  int ret;
  char *ptr;
  if (lstat(fullpath, &statbuf) < 0)	//获得文件状态失败
  {
    return(func(fullpath, &statbuf, FTW_NS));
  }
  if (S_ISDIR(statbuf.st_mode) == 0)	//如果不是目录
  {
    return(func(fullpath, &statbuf, FTW_F));
  }
  if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
  {
    return(ret);
  }
  ptr = fullpath + strlen(fullpath);	//指向路径缓冲区结尾
  *ptr++ = '/';
  *ptr = 0;
  if ((dp = opendir(fullpath)) == NULL)	//如果不能读目录
  {
    return(func(fullpath, &statbuf, FTW_DNR));
  }
  while ((dirp = readdir(dp)) != NULL) {
		if (strcmp(dirp->d_name, ".") == 0  ||
		    strcmp(dirp->d_name, "..") == 0)
				continue;		/* ignore dot and dot-dot */
		strcpy(ptr, dirp->d_name);	/* append name after slash */
		if ((ret = dopath(func)) != 0)		/* recursive */
			break;	/* time to leave */
	}
	ptr[-1] = 0;	/* erase everything from slash onwards */

	if (closedir(dp) < 0)
	{
		printf("can't close directory %s\n", fullpath);
    }
	return(ret);
}

static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
	switch (type) {
	case FTW_F:
		switch (statptr->st_mode & S_IFMT) {
		case S_IFREG:	nreg++;		break;
		case S_IFBLK:	nblk++;		break;
		case S_IFCHR:	nchr++;		break;
		case S_IFIFO:	nfifo++;	break;
		case S_IFLNK:	nslink++;	break;
		case S_IFSOCK:	nsock++;	break;
		case S_IFDIR:
			printf("for S_IFDIR for %s\n", pathname);
		}
		break;

	case FTW_D:
		ndir++;
		break;
	case FTW_DNR:
		printf("can't read directory %s\n", pathname);
		break;
	case FTW_NS:
		printf("stat error for %s\n", pathname);
		break;
	default:
		printf("unknown type %d for pathname %s\n", type, pathname);
	}
	return(0);
}


