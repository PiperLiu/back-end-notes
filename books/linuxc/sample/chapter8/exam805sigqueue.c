/*
使用sigqueue函数向进程自身发送一个SIGUSR1信号，
并获取该信号的信号值
*/
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
//SIGUSR1的处理函数
void signalDeal(int signo,siginfo_t *info,void *context)
{
    char *pMsg=(char*)info->si_value.sival_ptr;
    printf("接收到的信号标号是:%d\n", signo);
    printf("接收到信息:%s\n", pMsg);
}
//主函数
int main(int argc,char *argv[])
{
    struct sigaction sigAct;
    sigAct.sa_flags = SA_SIGINFO;
    sigAct.sa_sigaction = signalDeal;
    if(sigaction(SIGUSR1,&sigAct,NULL)==-1)
    {
        printf("sigaction函数调用失败!\n");
        exit(1);
    }
    sigval_t val;
    char pMsg[ ]="this is a test!";   //这是一段测试用的字符串
    val.sival_ptr = pMsg;
    if(sigqueue(getpid(),SIGUSR1,val) == -1)
    {
        printf("sigqueue函数调用失败!\n");
        exit(1);
    }
    sleep(3);     //休眠3秒
    return 0;
}
