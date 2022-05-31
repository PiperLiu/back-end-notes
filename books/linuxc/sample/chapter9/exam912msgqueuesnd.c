#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <error.h>

//信息结构体
struct my_msg
{
  long int my_msg_type;  //数据类型
  char text[BUFSIZ];     //消息缓冲区的大小
} msgbuf;

int main(int argc,char *argv[])
{
  int runningFlg =1;       //运行标志
  int msgid;            //消息标识符
  msgid = msgget((key_t)1234,0666 |IPC_CREAT); //创建一个消息队列，使用1234作为键值
  if(msgid==-1)
  {
    perror("创建消息队列失败!\n");  //如果创建失败
    exit(1);
  }
  while(runningFlg == 1)   //如果程序处于运行中
  {
    printf("输入希望发送的字符串: ");
    fgets(msgbuf.text,BUFSIZ,stdin);  //从标准输入读取BUFSIZ指定的数据
    msgbuf.my_msg_type = 1;  //指定数据类型
    if(msgsnd(msgid,(void *)&msgbuf, BUFSIZ, 0)==-1)  //发送数据
    {
      perror("发送消息失败!\n");  //如果发送失败
      exit(1);
    }
    if(strncmp(msgbuf.text,"end",3)==0)  //如果用户输入end
    {
      runningFlg = 0;   //结束运行
    }
   }
   return 0;
}

