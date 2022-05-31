#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc,char *argv[])
{
  int qid;
  int ret;
  if(argc < 2)
  {
    printf("请输入正确的键值\n");
    exit(0);
  }
  qid = atoi(argv[1]);  //获取键值
  ret = msgctl(qid,IPC_RMID,NULL);  //删除消息队列
  if(ret < 0)
  {
    printf("删除消息队列失败.\n");
    exit(0);
  }
  else
  {
    printf("删除消息队列成功.\n");
  }
  return 0;
}
