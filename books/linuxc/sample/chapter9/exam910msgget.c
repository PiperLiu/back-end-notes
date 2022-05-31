#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc,char *argv[])
{
  int qid;    //队列标志符
  key_t key;  //消息队列键值
  if(argc < 2)
  {
    printf("参数错误.\n");
    exit(0);
  }
  key = ftok(*(argv+1),'a');  //调用ftok函数生成队列键值
  if(key < 0)
  {
    printf("获取队列键值失败.\n");
    exit(0);
  }
  qid = msgget(key,IPC_CREAT | 0666);  //打开或者创建队列
  if(qid < 0)
  {
    printf("创建消息队列出错.\n");
    exit(0);
  }
  else
  {
    printf("创建消息队列成功.\n");
  }
  return 0;
}
