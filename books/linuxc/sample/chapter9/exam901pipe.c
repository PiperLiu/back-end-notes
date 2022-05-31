#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main(void)
{
    int fd[2];             // 文件描述符
    char writebuf[] = "this is a test!\n";    //写缓冲区
    char readbuf[20];      //读缓冲区
    if((pipe(fd)) < 0)     //创建管道
    {
        printf("创建管道失败!\n");
        exit(0);
    }
    write(fd[1],writebuf,sizeof(writebuf) );   //向管道写入端写入数据
    read(fd[0], readbuf, sizeof(writebuf) );   //从管道读出端读出数据
    printf ("%s",readbuf  ); //输出字符串
    printf ("管道的读fd是%d,管道的写fd是%d \n", fd[0], fd[1]) ;  //打印管道描述符
    close(fd[0]);              // 关闭管道的读出端文件描述符
    close(fd[1]);              // 关闭管道的写入端文件描述符
    return 0;
}

