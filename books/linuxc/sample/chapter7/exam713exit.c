/*体现exit和_exit的区别*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main(void)
{
    pid_t pid;
    if ( (pid = fork() )==-1 )   //如果创建子进程失败
     {
        perror ("创建子进程失败\n");  //创建子进程出错信息
        exit(0);
     }
    else if(pid==0)  //子进程
     {
        printf("01:这是子进程\n");
        printf("02:这是子进程，目前数据在缓冲区中"); 
        //这个地方没有换行符，所以不写出数据
        exit(0);   //退出，强制清空，会输出上面未完成数据
     }
    else  //父进程
     {
        sleep(1);   //休眠一秒以确定先后顺序
        printf("03:这是父进程，开始输出\n");
        printf("04:这是夫进程，目前数据在缓冲区中");  //同样没有换行符
        _exit(0);  //_exit函数会直接丢弃相应的数据
     }
     return 0;
}


