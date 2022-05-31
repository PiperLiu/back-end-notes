#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int  glob = 6;  //外部变量
int  main(int argc,char *argv[])
{
        int     var;    //内部变量
        pid_t   pid;    //文件标识符
        var = 88;	//内部变量
        printf("创建新进程之前。\n");    //还没有创建子进程
        if ((pid = vfork()) < 0)          //如果创建子进程失败
        {
          perror("创建子进程失败！");
        }
        else if (pid == 0)          //现在是子进程
        {                           
          glob++;                   //在子进程中修改变量值
          var++;
          //exit(0);
        }
        else                        //现在是父进程
        {
           //glob = 101;
           //var =  102;		    //修改变量的值
           //sleep(2);                 //父进程阻塞2秒
        }
        printf("进程标识符为= %d, glob = %d, var = %d\n", getpid(), glob, var);  
        //分别在子进程和输出两个变量的值
        exit(0);
}

