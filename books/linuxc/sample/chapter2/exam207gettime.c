//获得秒和微秒时间，现实和Greenwich的时间差，并且测试运行这段程序所需要的时间
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
int main(void)
{
        struct timeval time1,time2;
        struct timezone timez;
        gettimeofday(&time1,&timez);    //获得当前的时间
        printf("tv_sec; %d\n",time1.tv_sec);    //秒
        printf("tv_usec; %d\n",time1.tv_usec);  //毫秒
        printf("tz_minuteswest; %d\n",timez.tz_minuteswest);
        printf("tz_dsttime; %d\n",timez.tz_dsttime);
        gettimeofday(&time2,&timez);
        printf("time2_usec-time1_usec; %d\n",(time1.tv_usec - time2.tv_usec));
        //计算程序执行的时间
        return 0;
}

