/*
主进程休眠1秒，给子进程发送一个usr1信号,子进程接收到
usr1信号后进入注册信号处理函数，在屏幕上输出当前时间。
*/
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
//这是USR1的信号处理函数，用于在屏幕上输出时间信息
void singalUSR1Deal(int iSig)
{
  time_t timetmp;     //定义一个时间结构体变量
  if(iSig == SIGUSR1) //如果是用户信号1
  {
    time(&timetmp);     //获得当前时间参数
    printf("%s",ctime(&timetmp));  //在屏幕上输出当前时间
  }
  return;
}  
//以下为主函数
int main(int argc,char *argv[])
{
  pid_t pid;                  //进程的ID
  pid = fork();               //调用fork创建一个新的ID
  if(pid != 0)                //主进程
  {
    while(1)                  //循环
    {
      sleep(1);               //休眠1秒
      if(kill(pid,SIGUSR1) == -1)  //调用kill函数向子进程发送SIGUSR1信号
      {
         printf("向子进程发送SIGUSR1失败。\n");
         exit(0);            //退出
      }
    }
  }     
  else                       //子进程
  {
    signal(SIGUSR1,singalUSR1Deal);    //注册SIGUSR1
    while(1)
    {
    }
  }
}
