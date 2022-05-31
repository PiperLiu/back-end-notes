/*
主进程创建2个子进程，给子进程1每隔1秒发送一个usr1信号,子进程1接收到
usr1信号后进入注册信号处理函数，在屏幕上输出当前时间；给子进程2每隔
1秒发送一个usr2信号，子进程2接收到usr2信号后对一个在主进程中创建的文
件进行写入字符串操作
*/
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
//这是USR1的信号处理函数，用于在屏幕上输出时间信息

#define TRUE 0x01
#define FALSE 0x00

unsigned char flg = FALSE;     //标志位定义

void singalUSR1Deal(int iSig)
{
  time_t timetmp;     //定义一个时间结构体变量
  if(iSig == SIGUSR1) //如果是用户信号1
  {
    time(&timetmp);     //获得当前时间参数
    printf("%s",ctime(&timetmp));  //在屏幕上输出当前时间
  }
  return;
}
//这是USR2的信号处理函数，用于向一个文件中写入字符串
void singalUSR2Deal(int iSig)
{
  if(iSig == SIGUSR2)   //如果是用户信号2
  {
    if(flg == FALSE)   //如果标志为假
    {
      flg = TRUE;      //修改标志位
    }
  }
}

//以下为主函数
int main(int argc,char *argv[])
{
  pid_t pid1,pid2;            //进程的ID
  int fd;                     //文件描述符
  char writebuf[] = "this is a test!\n";   //待写入字符串
  int writecounter = 0;       //用于记录写入的偏移量
  int temp = 0,seektemp = 0,j = 0; //都是用于计算文件偏移的临时变量        
  if (argc != 2)              //如果参数不正确
  {
    printf("请输入正确的文件参数。\n");   
    return 0;
  }
  fd = open(*(argv+1),O_RDWR|O_CREAT,S_IRWXU); //打开或者创建一个文件
  pid1 = fork();              //调用fork创建一个新的进程
  if(pid1 != 0)                //主进程
  {
    pid2 = fork();             //创建第二个子进程
    if(pid2 != 0)             //主进程
    {
      while(1)                  //循环
      {
        sleep(1);               //休眠1秒
        if(kill(pid1,SIGUSR1) == -1)  //调用kill函数向子进程1发送SIGUSR1信号
        {
           printf("向子进程1发送SIGUSR1失败。\n");
           exit(0);            //退出
        }
        if(kill(pid2,SIGUSR2) == -1)  //调用kill向子进程2发送SIGUSR2信号
        {
           printf("向子进程2发送SIGUSR2失败。\n");
           exit(0);            //退出
        }
      }
    }
    else                       //这是子进程2的操作
    {
      signal(SIGUSR2,singalUSR2Deal);   //注册SIGUSR2的处理函数
      while(1)
      {
        while(flg == FALSE);          //如果标志为假则等待
        flg = FALSE;                  //修改标志位
        printf("这是子进程2.\n");     //屏幕输出提示
        if(writecounter == 0)        //第一次写入
        {
          temp = write(fd,writebuf,strlen(writebuf));  //写入数据
          seektemp = lseek(fd,0,SEEK_CUR);             //获得当前偏移量
          writecounter++; 
        }
        else
        {
          j = strlen(writebuf)*writecounter;
          seektemp = lseek(fd,j,SEEK_SET);
          temp = write(fd,writebuf,strlen(writebuf));
          writecounter++;
        }
      }
    }
  }
  else                       //子进程
  {
    signal(SIGUSR1,singalUSR1Deal);    //注册SIGUSR1的处理函数
    while(1)
    {
    }
  }
}

