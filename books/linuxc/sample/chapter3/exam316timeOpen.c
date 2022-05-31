//这是一个利用当前时间作为参数来创建新文件的应用
//新文件的格式为File+时+分+秒
//应用代码首先使用time系列函数获得当前的时、分、秒信息
//然后通过组合获得对应的字符串传递给Open函数创建文件
//最后在文件中写入一个含有时间参数的字符串
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
int main(void)
{
        time_t timetemp;        //定义一个时间结构体变量
        struct tm *p;           //结构体指针
        int i;
        char timebuf[7];        //时间信息，注意加上\0
        char writetimebuf[7];   //写文件时间缓冲区
        char filenamebuf[10] = "File";  //文件头
        char writebuf[30]="this is a test! the time is ";
        char enterbuf[3]="\r\n";       //回车换行buf
        int fd;
        int temp;
        time(&timetemp);          //获得时间参数
        printf("当前时间为%s",asctime(gmtime(&timetemp)));  //不需要添加回车换行符
        p = localtime(&timetemp);
        printf("%d:%d:%d\n",p->tm_hour,p->tm_min,p->tm_sec);
        sprintf(timebuf,"%02d%02d%02d",p->tm_hour,p->tm_min,p->tm_sec);
        //将时、分秒信息按照2位前端补0的方式格式化送入时间buf
        printf("step1 timebuf is %s\n",timebuf);
        strcpy(writetimebuf,timebuf);  //复制字符串
        printf("writetimebuf is %s\n",writetimebuf);
        strcat(filenamebuf,timebuf);
        printf("step2 timebuf is %s\n",timebuf);
        printf("filenamebuf is %s\n",filenamebuf);
        fd = open(filenamebuf,O_RDWR|O_CREAT,S_IRWXU);   //创建文件
        strcat(writebuf,writetimebuf);  //连接两个字符串
        strcat(writebuf,enterbuf);      //回车换行
        temp = write(fd,writebuf,strlen(writebuf));  //写入一个字符串以表示正确
        temp = close(fd);
        return 0;
}
