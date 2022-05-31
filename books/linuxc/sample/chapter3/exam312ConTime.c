//连续的每隔1秒打印系统的当前时钟
//使用sleep来进行不精确延时
#include <time.h>
#include <stdio.h>
int main(void)
{
      time_t timetemp;        //定义一个时间结构体变量
      while(1)
      {
        time(&timetemp);        //获得时间参数
        printf("%s",ctime(&timetemp));  //打印当前时间
        sleep(1);
      }
      return 0;
}
