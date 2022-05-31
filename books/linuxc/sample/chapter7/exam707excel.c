#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
  pid_t pid;   //进程标识符
  printf("**This is a test for exec series fun**\n"); //打印提示符
  if(fork()==0)  //创建一个子进程
  {
    execl("/bin/date","/bin/date",(char*)0);  //使用execl函数调用date命令
    exit(0);  //退出
  }
  else
 {
   sleep(1);  //主进程休眠
   execl("/bin/time","/bin/time",(char*)0);
 }
  exit(0);
}
