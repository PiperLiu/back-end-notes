//打开或者创建一个由agrv指定的文件
//然后使用dup函数复制该文件的描述符
//并且分别打印之前和复制之后的描述符
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(int argc, char * argv[])
{
  int fd;
  if(argc != 2)   //如果参数错误
  {
    printf("参数错误\n"); 
    return 1;
  }
  if((fd = open(*(argv+1),O_WRONLY|O_CREAT,0644)) == -1)  //打开argv1指定的文件
  {
    printf("打开文件%s失败\n",*(argv+1));  //打开函数错误
    return 2;
  }
  printf("当前文件描述符是%d\n",fd);       //打印当前的文件描述符w
  if( (fd = dup(fd)) == -1)                //获得新的文件描述符
  {
    printf("dup文件错误\n");      //dup函数操作错误
    return 3;
  }
  printf("dup文件成功!\n");           //dup操作成功
  printf("新的文件描述符是%d\n",fd);  //打印新的文件描述符
  close(fd);             //关闭文件
  return 0;
 }
