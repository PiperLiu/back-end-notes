#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
//这是线程处理函数
void *threaddeal(void *arg)
{
  printf("这是一个线程处理函数.\n");
  pthread_exit("pthread exit");    //线程退出
}
int main(int argc,char *argv[])
{
  pthread_t threadid;
  void *str;
  if(pthread_create(&threadid,NULL,threaddeal,NULL) != 0)  //创建线程
  {
    //创建线程失败
    printf("创建线程失败.\n");
    exit(0);
  }
  else   //创建线程成功
  {
    pthread_join(threadid,&str);    //等待新线程结束
    printf("%s\n",(char *)str);     //输出线程的退出状态
  }
  return 0;
}
