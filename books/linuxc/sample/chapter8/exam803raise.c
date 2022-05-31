#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
int main(void)
{
   printf("这是一个raise函数的应用实例\n");
   if(raise(SIGABRT) == -1)    //向进程本身发送SIGABRT信号失败
   {
     printf("调用raise函数失败!\n");   //提示发送失败，然后退出
     exit(1);
   }
   printf("raise发送SIGABRT信号没有成功!\n");   //如果进程被自己中止则不显示
   return 0;
}

