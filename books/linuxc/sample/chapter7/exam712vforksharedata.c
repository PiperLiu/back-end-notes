/*这是一个其分别利用子进程和父进程对一个count进行计数并且输出，
用于展示父进程和子进程是共享一个数据段*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main (int argc,char *argv[])
{
   int count = 1;
   int child;
   printf("此时执行的是父进程，当前count的值是:%d\n", count);   //创建子进程之前
   if(!(child = vfork()))   //创建子进程
     {  
        //由于子进程会首先执行，以下为子进程执行过程
        int i;
        for(i = 0; i < 100; i++)
          {
             printf("这是子进程, 当前i的值是: %d\n", i);   //反复输出打印结果
             if(i == 8)
             exit(1);
          }
         printf("这是子进程，其进程ID是%d count的值是: %d\n", getpid(), ++count);
         exit(1);  //退出子进程
      }
    else
     {  //父进程执行区
         printf("这是父进程，其进程ID是%d count的值是: %d, 其子进程是: %d\n", getpid(), count, child);
      }
return 0;
}
