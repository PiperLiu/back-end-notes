//这是一个使用utime函数对文件的时间参数进行修改的实例
//文件首先使用stat函数获得文件当前的时间参数，然后使用
//Open函数对文件进行修改，再用utime函数对文件的时间信息
//进行修改.文件名由argv参数给出
#include <stdio.h>
#include <fcntl.h>
#include <utime.h>
int main(int argc, char *argv[])
{
    int i, fd;
    struct stat	statbuf;  //文件信息缓冲区
    struct utimbuf timebuf;  //文件时间信息缓冲区
    for (i=1;i<argc;i++)    //支持同时对多个文件进行操作
    {
      if(stat(argv[i], &statbuf) < 0)   //获得文件当前信息失败
      {	
	printf("获取文件信息失败\n");      //输出提示并且进行到下一个文件
	continue;
      }
     if((fd = open(argv[i], O_RDWR|O_TRUNC)) < 0)  //尝试打开并且截断文件
     { 
	printf("打开截断文件操作失败\n");  //文件打开失败
	continue;
     }
     close(fd);  //关闭文件
     timebuf.actime  = statbuf.st_atime;  
     timebuf.modtime = statbuf.st_mtime;  //恢复时间
     if (utime(argv[i], &timebuf) < 0)    //复位时间失败
     {		
	printf("时间操作失败\n");     //复位时间失败
	continue;
     }
   }
   return 0;
}
