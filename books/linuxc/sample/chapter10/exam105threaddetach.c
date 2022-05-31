#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//线程处理函数
void *threaddeal(void *arg)
{
  int i = *(int *)(arg);
  printf("这是第%d个线程\n",i);
}
//主程序
int main(void)
{
  //线程id
  pthread_t threadid;
  int j;
  //创建大量线程
  int count = 20;//多次循环
  for(j=0 ; j < count ; j++)
  {
    //线程参数
    int * p = &(j);
    //创建线程
    int ret= pthread_create(&threadid, NULL, threaddeal, (void*)p);
    if(ret)//创建失败
    {
      printf("创建线程失败:%d\n",ret);
    }
    else//创建成功
    {
      //分离线程回收线程的stack占用的内存
      pthread_detach(threadid);
    }
  }
  return 0;
}
