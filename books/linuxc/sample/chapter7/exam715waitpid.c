#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
int main(void)
{
	pid_t pid;	
	if((pid=fork())<0)   //创建子进程失败
	{
		perror("创建子进程失败.\n");  //创建子进程失败
		exit(0);
	}
	else if(pid==0)  //进入子进程
	{
		if((pid=fork())<0)  //在子进程中继续创建一个子进程
		{
				perror("创建子进程失败.\n");
				exit(0);
		}
		else if(pid>0)   //当前创建子进程的父进程，即第一个子进程
                {
			exit(0);  //退出第一个子进程
                }
                else
                {
		  sleep(2);  //休眠2秒
		  printf("这是第二个子进程, parent pid=%d \n", getppid());
		  exit(0);
                }
	}
	
	if(waitpid(pid, NULL, 0)!=pid)   //判断到底是那个进程退出了
	{
		perror("waitpid销毁进程失败.\n");
		exit(0);
	}
	exit(0);
}
