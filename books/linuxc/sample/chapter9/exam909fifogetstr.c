#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>
#include <errno.h>

#define FIFO_FILE "MYFIFO"  //命名管道名称

int main(int argc,char * argv)
{
  FILE *fp;
  char readbuf[80];   //读缓冲区
  // 创建命名管道文件
  if((fp=fopen(FIFO_FILE,"r"))==NULL)
  {
    umask(0);
    mknod(FIFO_FILE,S_IFIFO|0666,0);
  }
  else
  {
    fclose(fp);  //如果存在关闭fp
  }
  while(1)
  {
   //打开命名管道文件
    if((fp = fopen(FIFO_FILE,"r"))==NULL)
    {
      printf("打开fifo失败. \n");
      exit(1);
    }
    // 从命名管道中读数据
    if(fgets(readbuf,80,fp)!=NULL)
    {
      printf("接收到字符串::%s \n", readbuf);
      fclose(fp);
    }
    else
    {
      if(ferror(fp))  //如果出错
      {
        perror("读文件失败.\n");
        exit(1);
      }
    }
  }
  return 0;
}

