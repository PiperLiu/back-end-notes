#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//线程处理函数
void *threaddeal(void *arg)
{
  printf("%d\n",*((int *)arg));  //传递线程的参数
  pthread_exit(NULL);
}

int main(int argc,char *argv[])
{
  int i;
  pthread_t threadid;
  for(i=0;i<10;i++)
  {
    if(pthread_create(&threadid,NULL,threaddeal,&i) != 0)  //将i值作为参数传递
    {
      //返回值不为0则表明创建线程失败
      printf("创建线程失败.\n");
      exit(0);    //退出
    }
  }
  pthread_exit(NULL);
  return 0;
}
