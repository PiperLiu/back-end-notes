#include <unistd.h>
#include <signal.h>
#include <stdio.h>
//SIGALRM的处理函数
void signalDeal(int sig)
{
  if(sig == SIGALRM)
  {
    printf("这是定时信号的处理函数!\n");
    return;
  }
}
//这是主函数
int main(int argc,char *argv[])
{
  int i = 0;
  signal(SIGALRM,signalDeal);   //注册SIGALRM的处理函数
  alarm(3);                     //3秒定时
  for(i=1;i<5;i++)
  {
    printf("sleeping %d ...\n",i);
    sleep(1);
  }
  return 0;
}

