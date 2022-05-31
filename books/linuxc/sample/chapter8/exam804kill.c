#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
int main(int argc,char *argv[])
{
  pid_t pid;
  pid = fork();      //创建子进程，进程ID存放在pid中
  if(pid == 0)       //子进程
  {
    printf("这是子进程!\n");
    sleep(10);                        //休眠10秒
    printf("子进程没有收到退出指令!\n");  //如果接收到SIGABRT不会打印
    return;
  }
  else              //这是父进程
  {
    printf("父进程调用kill函数向子进程%d发送SIGABRT信号\n",pid);
    sleep(1);      //休眠1秒
    if(kill(pid ,SIGABRT) == -1)  //如果调用kill函数失败
    {
      printf("调用kill函数失败!\n");
    }
  }
  return 0;
}

