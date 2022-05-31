#include <sys/types.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>

int main(int argc,char *argv[])  
{  
    FILE   *stream;  
    FILE   *wstream;       //定义两个文件流
    char   buf[1024];      //定义以缓冲区
    if(argc != 2)          //如果文件参数不正确
    {
      printf("请输入正确的文件参数\n");
      exit(1);   
    }
    memset(buf,'a',sizeof(buf));  //初始化buf,以免后面写如乱码到文件中
    stream = popen("ls -l", "r" );    //将“ls －l”命令的输出 通过管道读取（“r”参数）到FILE* stream
    wstream = fopen(*(argv+1), "w+"); //新建一个指定的的文件
    fread(buf, sizeof(char), sizeof(buf),  stream); 
     //将刚刚FILE* stream的数据流读取到buf中
    fwrite(buf, 1, sizeof(buf), wstream );//将buf中的数据写到FILE*wstream对应的流中，也是写到文件中
    pclose(stream );  
    fclose(wstream );    //关闭退出
    return 0;
}  
