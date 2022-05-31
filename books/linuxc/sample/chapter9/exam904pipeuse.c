/*
主进程创建2个子进程，子进程1每隔1秒向子进程2发送一个
字符串，子进程接收到该字符串之后将其写入一个指定的文件
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

//以下为主函数
int main(int argc,char *argv[])
{
  pid_t pid1,pid2;            //进程的ID
  int fd;                     //文件描述符
  char writebuf[] = "this is a test!\n";   //待写入字符串
  char readbuf[25];            //读缓冲区
  int writecounter = 0;       //用于记录写入的偏移量
  int temp = 0,seektemp = 0,j = 0; //都是用于计算文件偏移的临时变量
  int pipefd[2];              //管道的文件描述符
  if (argc != 2)              //如果参数不正确
  {
    printf("请输入正确的文件参数。\n");
    return 0;
  }
  fd = open(*(argv+1),O_RDWR|O_CREAT,S_IRWXU); //打开或者创建一个文件
  if(pipe(pipefd) < 0)        //如果创建管道失败
  {
    printf("创建管道失败。\n");
    exit(0);                  //退出
  }
  pid1 = fork();              //调用fork创建一个新的进程
  if(pid1 != 0)                //主进程
  {
    pid2 = fork();             //创建第二个子进程
    if(pid2 != 0)             //主进程
    {
      close(pipefd[0]);          //关闭管道
      close(pipefd[1]);
    }
    else                       //这是子进程2的操作
    {
      close(pipefd[1]);
      while(1)
      {
        read(pipefd[0],readbuf,sizeof(writebuf));   //读管道 
        printf("这是子进程2.\n");     //屏幕输出提示
        if(writecounter == 0)        //第一次写入
        {
          temp = write(fd,readbuf,strlen(readbuf));  //写入数据
          seektemp = lseek(fd,0,SEEK_CUR);             //获得当前偏移量
          writecounter++;
        }
        else
        {
          j = strlen(readbuf)*writecounter;
          seektemp = lseek(fd,j,SEEK_SET);
          temp = write(fd,readbuf,strlen(writebuf));
          writecounter++;
        }
      }
    }
  }
  else                       //子进程
  {
    close(pipefd[0]);
    while(1)
    {
      sleep(1);
      write(pipefd[1],writebuf,sizeof(writebuf));   //将字符串写入管道
    }
  }
}
