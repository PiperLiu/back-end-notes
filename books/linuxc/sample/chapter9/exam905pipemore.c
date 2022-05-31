#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>
#include <error.h>

#define DEF_PAGER "/bin/more"   //定义处理函数
#define MAXLINE  4096   //行最大字符数

int main(int argc, char *argv[])
{
  int     n;
  int     fd[2];
  pid_t   pid;
  char    *pager, *argv0;
  char    line[MAXLINE];
  FILE    *fp;
  if (argc != 2)  //如果参数不正确
  {
    printf("请输入正确的命令:<pathname>\n");
    exit(1);
  }
  if ((fp = fopen(argv[1], "r")) == NULL)  //如果以只读打开argv[1]指向的文件出错
  {
    printf("不能打开文件%s", argv[1]);
    exit(1);
  }
  if (pipe(fd) < 0)  //创建管道失败
  {
    printf("创建管道失败\n");
    exit(0);
  }
  if ((pid = fork()) < 0)  //创建子进程失败
  {
    printf("创建子进程失败\n");
    exit(0);
  }
  else if (pid > 0)
  {                               //父进程
    close(fd[0]);           //关闭读文件描述符
    //将argv[1]通过管道发送
    while (fgets(line, MAXLINE, fp) != NULL)
    {
      n = strlen(line);
      if (write(fd[1], line, n) != n)
      {
        printf("写管道失败\n");
        exit(1);
      }
    }
    if (ferror(fp))  //如果文件描述符出错
    {
        printf("fgets 失败\n");
        exit(1);
    }
    close(fd[1]);   //
    if (waitpid(pid, NULL, 0) < 0)
    {
        printf("waitpid失败\n");
        exit(1);
    }
    exit(0);
  }
  else  //子进程
  {
    close(fd[1]);
    if (fd[0] != STDIN_FILENO)
    {
      if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
      {
        printf("dup2到标准输入失败\n");
        exit(1);
      }
      close(fd[0]);   /* don't need this after dup2 */
    }
    //exec函数的参数
    if ((pager = getenv("PAGER")) == NULL)
    {
      pager = DEF_PAGER;
    }
    if ((argv0 = strrchr(pager, '/')) != NULL)
    {
      argv0++;
    }
    else
    {
      argv0 = pager;
    }
    if (execl(pager, argv0, (char *)0) < 0)
    {
      printf("调用execl失败%s", pager);
      exit(1);
    }
  }
  exit(0);
}
