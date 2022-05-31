#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <error.h>
char command[256]; 
void main() 
{ 
  int rtn; /*子进程的返回数值*/ 
  while(1) 
  { 
    printf( ">" );              //从终端读取要执行的命令 
    fgets(command, 256, stdin); //将命令数据存放到command中
    command[strlen(command)-1] = 0; 
    if (fork() == 0 )          //在子进程中执行这个命令
    {  
      execlp(command,command,NULL); 
      //如果exec函数返回，表明没有正常执行命令，打印错误信息
      perror(command); 
      exit(1); 
    } 
    else  //在父进程中等待字进程结束，并且打印子进程的返回值
    { 
      wait( &rtn ); 
      printf("子进程返回%d\n",rtn); 
      exit(0);
    } 
  } 
  exit(0);
} 
