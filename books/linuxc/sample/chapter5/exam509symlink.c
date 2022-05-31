#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
  int ret;              //存放返回值
  if(argc != 3)         //判断参数数目
  {
    printf("请输入3个参数\n");
    exit(0);
  }	  
  ret = symlink(argv[1],argv[2]);  //创建符号链接
  if(ret != 0)  //创建符号链接文件失败
  {
    perror("创建符号链接失败!\n");
    exit(0);
  }	  
  return(0);
}
