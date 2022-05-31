#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#define BUFSIZE 4096  //定义一个最大的读写空间
int main(void)
{
    int fd[2];
    char buf[BUFSIZE] = "hello!I am your  brother!\n";        // 缓冲区
    pid_t pid;
    int len;
    if ( (pipe(fd)) < 0 )           //创建管道
     {
        perror("pipe failed\n");
     }
    if ( (pid = fork()) < 0 )        //创建第一个子进程
     {
        perror("fork failed\n");
     }
    else if ( pid == 0 )            //子进程
    {
        close ( fd[0] );                //关闭不使用的文件描述符
        write(fd[1], buf, strlen(buf));   //发送字符串
        exit(0);
     }
    if ( (pid = fork()) < 0 )             //创建第二个子进程
     {
        perror("fork failed\n");
     }
    else if ( pid > 0 )             //父进程
    {
        close ( fd[0] );
        close ( fd[1] );
        exit ( 0 );
     }
    else                       //第二个子进程中
    {
        close ( fd[1] );                  //关闭管道文件描述符
        len = read (fd[0], buf, BUFSIZE);  //读取消息
        write(STDOUT_FILENO, buf, len); //将消息输出到标准输出
        exit(0);
     }
    return 0;
}
