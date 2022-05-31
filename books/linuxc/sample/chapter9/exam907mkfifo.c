#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char *argv[])
{
  mode_t mode = 0755;             //文件的权限设置
  if(argc != 2)
  {
    printf("请输入正确的文件参数.\n");
    exit(0);
  }
  if(mkfifo(*(argv+1),mode) < 0)  //创建FIFO失败
  {
    printf("创建fifo失败.\n");
    exit(1);
  }
  else
  {
    printf("创建fifo成功.\n");
  }
  return 0;
}
