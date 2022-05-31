/*这是一个调用fork函数创建子进程的实例，当创建进程成功之后会分别
打印两者对应的进程标识符*/
#include <stdio.h>
#include <stdlib.h>
int main (void)
{
   pid_t pid;         //进程标识符
   pid = fork();      //创建一个新的进程
   if(pid < 0)        //如果返回的pid小于0则标示创建进程失败
     {
        printf("创建进程失败!");
        exit(1);    //fork出错，退出
     }
   else if(pid == 0)  //如果pid为0则表示当前执行的是子进程
     printf("这是子进程，进程标识符是%d\n",getpid());
   else               //否则为夫进程
     printf("这是父进程，进程标识符是%d\n",getpid());
   return 0;      //返回
}
