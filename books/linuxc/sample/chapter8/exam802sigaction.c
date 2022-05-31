#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
//这是使用sigaction函数注册的信号函数
void signalDeal(int sig,siginfo_t *info,void *t)
{
  if(sig == SIGINT)    //对应ctrl+c
  {
    printf("CTRL+C按键被按下。\n");
  }
  else if(sig == SIGQUIT)  //对应ctrl+/
  {
    printf("CTRL+/按键被按下.\n");
  }
  else
  {
    printf("其他信号。\n");
  }
}
int main(int argc,char *argv[])
{
  struct sigaction act;    //定义sigaction结构体
  act.sa_sigaction = signalDeal;  //指定信号处理函数
  sigemptyset(&act.sa_mask);   //清空信号集中的信号
  act.sa_flags = SA_SIGINFO;    //信号附带的参数可以被传递到处理函数中
  sigaction(SIGINT,&act,NULL);  //设置SIGINT处理函数
  sigaction(SIGQUIT,&act,NULL); //设置SIGQUIT处理函数
  while(1)
  {
  }
  return 0;
}


