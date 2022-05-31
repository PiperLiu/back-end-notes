#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define TRUE 1     //定义两个常量
#define FALSE 0 
int thread_flag = TRUE;   //标志位
//线程处理函数
void *threaddeal(void *arg) 
{
  printf("当前线程正在执行.\n");
  sleep(3);                                //休眠3秒
  printf("线程即将退出.\n");
  thread_flag = FALSE;                     //修改线程标志位
  pthread_exit(NULL);                      //线程退出
}
//主程序
int main(int argc,char *argv[])
{
  pthread_t threadid;                    //定义线程描述符
  pthread_attr_t thread_attr;            //定义线程属性对像
  pthread_attr_init(&thread_attr);       //线程属性初始化
  pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);    //将线程设置为分离状态
  if(pthread_create(&threadid, &thread_attr, threaddeal, NULL))    //创建新线程，并修改属性
  {
    printf("创建线程失败\n");
    exit(0);
  }
  while(thread_flag)    //判断标志位
  {
    printf("等待线程结束\n");
    sleep(1);
  }
  printf("线程结束.\n");
  return 0;
}

