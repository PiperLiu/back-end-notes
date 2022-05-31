//打印系统的当前时钟
#include <time.h>
#include <stdio.h>
int main(void)
{
        time_t timetemp;        //定义一个时间结构体变量
        char *wday[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
        struct tm *p;           //结构体指针
        time(&timetemp);        //获得时间参数
        printf("%s",asctime(gmtime(&timetemp)));
        p = localtime(&timetemp);
        printf("%d:%d:%d:\n",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday);
        printf("%s  %d:%d:%d\n",wday[p->tm_wday],p->tm_hour,p->tm_min,p->tm_sec);
        return 0;
}

