#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//这是对信号的处理函数
static void signalDeal(int signo)
{
   struct timeval tp;
   struct tm *tm;
   gettimeofday(&tp,NULL);   //获得系统当前时间（秒和微秒）
   tm=localtime(&tp.tv_sec);   //获得当地目前时间和日期
   printf(" sec = %ld \t",tp.tv_sec);    //打印从UNIX纪元开始到现在的秒数
   printf(" usec = %ld \n",tp.tv_usec);  //打印微秒 printf("%d-%d-%d%d:%d:%d\n",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);  /*打印当地目前时间和日期*/
}
//时间初始化函数
static void InitTime(int tv_sec,int tv_usec)
{
   struct itimerval value;        //定义时间参数结构体value
   signal(SIGALRM, signalDeal);  //注册信号SIGALRM 和信号处理函数
   value.it_value.tv_sec = tv_sec;   //秒
   value.it_value.tv_usec = tv_usec; //微秒
   value.it_interval.tv_sec = tv_sec;
   value.it_interval.tv_usec = tv_usec;
   setitimer(ITIMER_REAL, &value, NULL);
   //setitimer 发送信号，定时类型为ITIMER_REAL
}
//主函数
int main(int argc,char *argv[])
{
   InitTime(1,0);     //每隔1秒打印一次
   while(1)
   {
   }
   exit(0);
}

