#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;    //定义一个互斥量             
int x;                    //定义一个全局变量                                
//这是线程1的入口函数
void threaddeal1(void)                
{
  while(x>0)              //如果X>0
  {
    pthread_mutex_lock(&mutex);        //对互斥量进行加锁操作       
    printf("线程1正在运行: x=%d \n",x); //输出当前的x值
    x--;                                //将x的值-1
    pthread_mutex_unlock(&mutex);       //对互斥两进行开锁操作
    sleep(1);                           //休眠1秒
  }
  pthread_exit(NULL);                   //进程退出
}
//这是线程2的入口函数，线程2和线程1的操作完全相同
void threaddeal2(void) 
{
  while(x>0)
  {
    pthread_mutex_lock(&mutex); 
    printf("线程2正在运行: x=%d \n",x);
    x--;
    pthread_mutex_unlock(&mutex);
    sleep(1);
  }
  pthread_exit(NULL);
}
//这是主函数
int main(int argc,char *argv[])
{
  pthread_t threadid1,threadid2;                        
  int ret;
  ret = pthread_mutex_init(&mutex,NULL);   //初始化互斥锁
  if(ret != 0)
  {
    printf ("初始化互斥锁失败.\n"); 
    exit (1);
  }
  x = 10;     //给全局变量赋初始化值                                
  ret = pthread_create(&threadid1, NULL, (void *)&threaddeal1, NULL);  //创建线程1    
  if(ret != 0)
  {
    printf ("创建线程1失败.\n");
    exit (1);
  }
  ret = pthread_create(&threadid2, NULL, (void *)&threaddeal2, NULL);  //创建线程2   
  if(ret != 0)
  {
    printf ("创建线程2失败.\n");
    exit (1);
  }
  pthread_join(threadid1, NULL); 
  pthread_join(threadid2, NULL);    //阻塞线程1和线程2
  return (0);
}

