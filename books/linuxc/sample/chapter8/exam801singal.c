#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
//这是信号处理函数
void signalDeal(int sig)
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
//以下是主函数
int main(int argc,char *argv[])
{
  signal(SIGINT,signalDeal);   //注册SIGINT对应的处理函数
  signal(SIGQUIT,signalDeal);  //注册SIGQUIT对应的处理函数
  while(1)    //永远循环
  {
  }
  return 0;
}
