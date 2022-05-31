#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

//定义的消息队列的结构体
struct my_msg
{
  long int my_msg_type;
  char text[BUFSIZ];
} msgbuf;

int main(int argc,char *argv[])
{
  int runningFlg =1;
  int msgid;
  long int msg_to_receive=0;
  msgid = msgget((key_t)1234,0666 |IPC_CREAT);  //建立消息队列
  if(msgid == -1)  //如果建立消息队列失败
  {
    printf("msgget failed!\n");
    exit(1);
  }
  while(runningFlg == 1)  //进入循环
  {
     if(msgrcv(msgid,(void *)&msgbuf, BUFSIZ,msg_to_receive, 0)==-1)
     {
        perror("msgrcv failed!\n");   //如果接收数据失败
        exit(1);
     }
     printf("接收到的字符串是 : %s", msgbuf.text);
     if(strncmp(msgbuf.text,"end",3)==0)
     runningFlg  = 0;  //如果接收完成
  }
  if(msgctl(msgid, IPC_RMID, 0)==-1)  //删除消息队列
  {
    perror("msgctl(IPC_RMID)  failed!\n");  //如果删除消息队列失败
    exit(1);
  }
  return 0;
}

