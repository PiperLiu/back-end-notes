//这是一个使用read函数把目标文件中数据读出写入到另外一个文件中的实例
//待读出数据文件由argv1参数给，待写入数据文件由argv2给出
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#define PERMS 0666        
#define	DUMMY 0
#define MAXSIZE 1024         //常数定义
int main(int argc, char *argv[])
{
  int sourcefd, targetfd;  //目标文件和源文件的描述符
  int readCounter = 0;	   //读出的字符计数器					
  char WRBuf[MAXSIZE];     //读写缓冲区
  if(argc!=3)              //如果命令行参数不争取正确
  {
    printf("Plz input the correct filename as './exam311ReadFun filename1 filename2'\n");
    return 1;
  }
  if((sourcefd = open(*(argv+1),O_RDONLY,DUMMY))==-1)   //如果源文件打开失败
  {
    printf("Source file open error!\n");
    return 2;
  }
  if((targetfd = open(*(argv+2), O_WRONLY|O_CREAT, PERMS))==-1)  //如果目标文件打开失败
  {
    printf("Target file open error!\n");
    return 3;
  }
  while(( readCounter = read(sourcefd, WRBuf, MAXSIZE))>0)  //如果读出来的数据大于0
  {
    if(write(targetfd, WRBuf,readCounter) != readCounter)   //如果写入的数据和读出的数据不同
    {
      printf("Target file write  error!\n");  //写数据错误
      return 4;
    }
  }
  close(sourcefd);  //关闭源文件
  close(targetfd);  //关闭目标文件
  return 0;
}

