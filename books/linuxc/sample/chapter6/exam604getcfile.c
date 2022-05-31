//使用fopen打开指定文件
//调用getc读出数据并且显示到屏幕
//将一个字符串写入该文件 
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 

int main(int argc,char *argv[])  
{  
    int ch;  
    int len;       //写入缓冲区的长度计数器
    int i = 0;  
    FILE *fp;     //文件结构指针  

    char writebuf[] = "Hello!I have read this file.\r\n";  //写入缓冲区  
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
    while( (ch = getc(fp)) != EOF)  
    {  
        putchar(ch);   //在显示器上输出字符
    }  
    //putchar('\n');     //回车换行
    len = strlen(writebuf); //获得写入缓冲区的实际长度  
    while(len > 0)            //循环写入数据
    {  
        putc(writebuf[i],fp);  //写入数据  
        putchar(writebuf[i]);  //显示数据 
        len--;  
        i++;                   //更新计数器  
    }   
    fclose(fp);               //关闭计数器  
    return 0;  
} 
