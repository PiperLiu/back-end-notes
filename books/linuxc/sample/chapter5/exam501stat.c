#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
int main(int argc, char *argv[])
{
  int ret;
  struct stat stat_buf;    //定义stat结构体变量
  if(argc != 2)            //检查命令行参数
  {
    printf("请输入正确的文件参数！\n");
    return 0;
  }
  ret = stat(argv[1], &stat_buf);        //获取文件属性
  if(ret == -1)                          //获取文件属性失败
  {
    perror("获取文件属性失败！\n");
    exit(0);
  }
  switch(stat_buf.st_mode & S_IFMT)        
  //判断文件类型，S_IFMT是st_mode中文件类型的屏蔽码
  {
    case S_IFDIR:                        //目录文件
    printf("这是一个目录文件！\n");
    break;
    case S_IFREG:                        //普通文件
    printf("这是一个普通文件！\n");
    break;
  }    
  return 0;
}
