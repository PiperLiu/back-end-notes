#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc,char *argv[])
{
  int n, fd[2];            
  pid_t pid;
  char buffer[25];  //缓冲区
  if(pipe(fd)<0)    //创建一个管道，两个文件描述符在fd数组中
  {
    printf("创建管道失败!\n ");
    exit(0);
  }
  if((pid=fork())<0)    //创建一个子进程
  {
    printf("创建子进程失败!\n ");
    exit(0);
  }
  else if (pid>0)   //父进程
  {
    close(fd[0]);
    write(fd[1],"This is a pipe test!\n",22);  //向管道写入数据,注意回车换行符
  }
  else  //子进程
  {
    close(fd[1]); //关闭
    n = read(fd[0],buffer,25);  //从通道中读出数据
    printf("%s",buffer);  //将数据写到标准输出设备
  }
  exit(0);
}

