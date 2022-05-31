#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
//线程1的启动函数
void *threaddeal1(void *arg)
{
  printf("thread 1 returning\n");
  return((void *)1);
}
//线程2的启动函数
void *threaddeal2(void *arg)
{
   printf("thread 2 exiting\n");
   pthread_exit((void *)2);
}
//主函数
int  main(int argc,char *argv[])
{
   int             err;
   pthread_t       threadid1, threadid2;
   void            *tret;
   //创建线程1
   err = pthread_create(&threadid1, NULL, threaddeal1, NULL);
   if (err != 0)   //创建线程1失败
   {
     printf("创建线程1失败，错误为: %s\n", strerror(err));
   }
   //创建线程2
   err = pthread_create(&threadid2, NULL, threaddeal2, NULL);
   if (err != 0)
   {
     printf("创建线程2失败，错误为: %s\n", strerror(err));
   }
   //阻塞线程1
   err = pthread_join(threadid1, &tret);
   if (err != 0)
   {
     printf("阻塞线程1失败，错误为: %s\n", strerror(err));
   }
   //退出并且打印线程1的退出状态
   printf("线程1的退出码为 %d\n", (int)tret);
   //阻塞线程2
   err = pthread_join(threadid2, &tret);
   if (err != 0)
   {
     printf("阻塞线程2失败，错误为: %s\n", strerror(err));
   }
   //退出并且打印线程2的退出状态
   printf("线程2的退出码为 %d\n", (int)tret);
   exit(0);
}
