/*子进程和父进程同时对一个文件进行写操作导致文件发生混乱的实例
写入数据的文件由argv[1]指定，数据来源在argv[2]所指定的文件中*/
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
int readfd, writefd;    //读文件描述符和写文件描述符
char c;                 //文件内容的中转字符空间
int main(int argc, char*argv[])
{
  if(argc!=3)   //如果参数不正确
  {
    printf("Usage %s sourcesfiel destfile. \n",argv[0]);
    return 1;
  }
  if((readfd = open(argv[1], O_RDONLY))==-1)  //如果打开文件失败
  {
    printf("打开文件%s失败！\n",argv[1]);
    return 2;
  }
  if((writefd = creat(argv[2],S_IRWXU))==-1)   //如果创建文件失败
 {
    printf("创建文件%s失败！\n",argv[2]);
    return 3;
 }
 fork();   //创建子进程，以下为父进程和子进程同时执行的步骤
 for(;;)
 {
   if(read(readfd,&c,1) != 1)  //如果读不出数据则返回
   {
     return 4;
   }
   write(writefd,&c,1);    //将读出的数据写入文件中
 }
 return 0;
}
