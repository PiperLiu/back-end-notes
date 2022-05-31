#include <stdio.h>
#include <stdlib.h>
int main (void)
{
   pid_t pid1,pid2;   //进程标识符
   pid1 = fork();      //创建一个新的进程
   if(pid1 < 0)        //如果返回的pid小于0则标示创建进程失败
     {
        printf("创建进程失败!\n");
        exit(1);    //fork出错，退出
     }
   else if(pid1 == 0)  //如果pid为0则表示当前执行的是子进程
   {
     printf("这是子进程1,进程标识符是%d\n",getpid());
   }
   else               //否则为夫进程
   {
     printf("这是父进程，进程标识符是%d\n",getpid());
     pid2 = fork();
     if(pid2 < 0)
     {
        printf("创建第二个进程失败！\n");
        exit(2);
     }
     else if(pid2 == 0) //第二个子进程
     {
        printf("这是子进程2，进程标识符是%d\n",getpid()); 
     }
     else
     {
        printf("这是子进程2的父进程，进程标志是%d\n",getpid());
     }
   }
   printf("这是一个多进程测试，即将退出!\n");
   return 0;      //返回
}
