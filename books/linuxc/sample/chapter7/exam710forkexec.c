#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{ 
    pid_t pid;
    pid = fork();
    if(pid == 0)   //子进程
    {
      execl("/bin/ls","ls","-al","/etc/passwd",(char * )0);  
    //执行bin下的ls命令，查找etc下的passwd文件，参数为ls
      exit(0);
    }
    else
    {
      execl("/bin/ls","ls","-al","./exam702fork.c",(char *)0);
    //执行bin下的ls命令，查找当前文件夹下的exam81getpid.c文件
      exit(1);
    }
    return 0;
}
