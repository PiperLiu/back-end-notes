//这是一个低效率的使用gettimeofday来获得秒定时的应用
//使用gettimeofday在while循环中连续获得当前的timez信息
//然和和之前的时间信息进行比较，如果还没到1秒，则等待，否则
//使用break跳出while循环并且打印当前时间，实现每秒打印一次
#include<sys/time.h>
#include<stdio.h>
int main(void)
{
  struct timeval timenow,timeold;
  struct timezone timez;
  time_t timetemp;    //时间结构体变量
  gettimeofday(&timeold,&timez);   //取得一个时间信息作为以前的数据
  while(1)
  {
    while(1)
    {
  	gettimeofday(&timenow,&timez);   //获得当前时间数据
        if((timenow.tv_sec - timeold.tv_sec) == 1)  //如果时间过了一秒
        {
          timeold = timenow;		//更新以前的时间参考数据
          break;                        //退出当前循环
        }
     }  
     //如果还没到1秒，则一直等待；
     time(&timetemp);    //获得时间参数
     printf("%s",ctime(&timetemp));     //打印当前时间
  }
  return 0;
}
