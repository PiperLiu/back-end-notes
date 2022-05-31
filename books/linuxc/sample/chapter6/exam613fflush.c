#include <stdio.h>    
char inbuf[BUFSIZ];   
//BUFSIZ是stdio.h的一个关于缓冲区的预定义，大小为8192   
int main(int argc,char *argv[])    
{    
    char temp[100];      //临时缓冲区  
    setbuf(stdin, inbuf);       //将inbuf设置为标准输入的缓冲区
    printf("请输入一个字符串 =");   
    scanf("%s",temp);   //将键盘输入读入临时缓冲区
    puts(inbuf);                  //往缓冲区写入数据  
    //if(0 == fflush(stdin))        //冲洗文件缓冲区  
    if(0 == fflush(inbuf))
    {  
        puts(inbuf);     //如果冲洗成功，将缓冲区的输入写入  
    }  
    puts(inbuf);         //再次写入缓冲区数据
    return 0;    
}  
