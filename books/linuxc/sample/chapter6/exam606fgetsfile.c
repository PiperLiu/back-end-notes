#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 

#define MAXLINE 4096    //定义一行字符的最大长度
int main(int argc,char *argv[])  
{  
    char buf[MAXLINE];   //读写缓冲区  
    int len;       //写入缓冲区的长度计数器
    int i = 0;  
    FILE *fp;     //文件结构指针  

    char writebuf[] = "Hello!I have read this file.\n";  //写入缓冲区  
    if(argc != 2)
    {
      printf("请输入正确的参数/n");  //参数错误
      return 1;
    }
    fp = fopen(*(argv+1),"ab+");     //打开指定稳定 
    if(fp == NULL)  
    {  
        printf("打开文件%s失败!\n",*(argv+1));  
        return 2;  
    }  
    //从文件中读取数据，直到到文件末位  
    while((fgets(buf,MAXLINE,fp)) != NULL)    //如果没有到文件末位  
    {  
        fputs(buf,stdout);   //在标准输出中输出字符
    } 
    fputs(writebuf,fp);      //将写入缓冲区的数据写入文件   
    fclose(fp);               //关闭计数器  
    return 0;  
} 
