/*这是一个调用fork函数创建一个子进程，然后分别打印输出子进程
和父进程中的变量的实例*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int  glob = 6;  //外部变量
int  main(void)
{
        int     var;    //内部变量
        pid_t   pid;    //文件标识符
        var = 88;	//内部变量
        printf("创建新进程之前。\n");    //还没有创建子进程
        if ((pid = fork()) < 0)          //如果创建子进程失败
        {
          perror("创建子进程失败！");
        }
        else if (pid == 0)          //现在是子进程
        {                           
          glob++;                   //在子进程中修改变量值
          var++;
        }
        else                        //现在是父进程
        {
          sleep(2);                 //父进程阻塞2秒
        }
        printf("进程标识符为= %d, glob = %d, var = %d\n", getpid(), glob, var);  
        //分别在子进程和输出两个变量的值
        exit(0);
}

