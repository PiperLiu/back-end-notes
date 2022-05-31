#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//新线程首先运行的函数
void *threaddeal(void *arg)
{
  printf("这是一个新线程.\n");  //输出新线程提示
}
int main(int arg,char *argv[])
{
  pthread_t threadid;   //线程的标识符
  if(pthread_create(&threadid,NULL,threaddeal,NULL) != 0)   
  //创建一个新县城，然后运行threaddeal函数
  {
    //如果返回值不是0则表示创建线程失败
    printf("%s错误出现在第%s行",__FUNCTION__,__LINE__);  //打印错误信息
    exit(0);
  }
  else
  {
    sleep(1);    //挂起1秒等待线程运行
  }
  return 0;
}
