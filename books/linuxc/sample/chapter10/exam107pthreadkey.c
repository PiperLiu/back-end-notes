#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#if 0    //预定义
char * str_accumulate(char *s)
{
    static char accu[1024]={0};
    strcat(accu,s);
    return accu;
}
#endif
  static pthread_key_t str_key;     //顶一个键值
  static pthread_once_t str_alloc_key_once = PTHREAD_ONCE_INIT;  //用于解决键冲突
  static void str_alloc_key();  //按键分配函数
  static void str_alloc_destroy_accu(void *accu);  //撤销按键分配函数
  //处理函数
  char * str_accumulate(const char *s)
  {
    char *accu;
    pthread_once(&str_alloc_key_once,str_alloc_key);  //解决按键冲突
    accu = (char *)pthread_getspecific(str_key);      //获取线程的私有数据地址
    if(accu == NULL)
    {
      accu = malloc(1024);   //分配1024的空间
      if(accu == NULL)  //如果accu为NULL则直接返回NULL
      {
        return NULL;
      }
      accu[0] = 0;
      pthread_setspecific(str_key,(void *)accu);   //将accu存放的数据作为键值关联
      printf("Thread %lx : allocating buffer at %p\n",pthread_self(),accu); //打印输出
    }
    strcat (accu,s);  //将accu和s字符串连接到一起
    return accu;
  }
  //这是一个键值分派函数
  static void str_alloc_key()
  {
    pthread_key_create(&str_key,str_alloc_destroy_accu);   //创建键值
    printf("Thread %lx : allocated key %d\n",pthread_self(), str_key);
  }
  //这是撤销键值的函数
  static void str_alloc_destroy_accu(void *accu)
  {
    printf("Thread %lx : freeing buffer at %p\n",pthread_self(),accu);
    free(accu);      //释放空间
  }
  //线程处理函数
  void *threaddeal(void *arg)
  {
    //该函数的主要工作是将arg的字符串和“Result of和thread连接到一起”
    char *str;
    str = str_accumulate("Result of ");
    str = str_accumulate((char *)arg);
    str = str_accumulate(" thread");
    printf("Thread %lx: \"%s\" \n",pthread_self(),str);
    return NULL;
  }
//主函数
int main(int argc, char *argv[])
{
  char *str;
  pthread_t th1,th2;
  str = str_accumulate("Result of ");
  pthread_create(&th1,NULL,threaddeal,(void *)"first");
  pthread_create(&th2,NULL,threaddeal,(void *)"second");  //建立两个线程
  str = str_accumulate("initial thread");
  printf("Thread %lx :\"%s\"\n",pthread_self(),str);
  pthread_join(th1,NULL);
  pthread_join(th2,NULL);   //阻塞线程1和线程2
  return 0;
}

