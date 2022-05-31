//使用fgets从标准输入读入一行数据
//然后使用fputs送标准输出显示
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 4096      //定义一行的最大字符长度
int main(int argc,char *argv[])
{
  char buf[MAXLINE];     //缓冲区大小
  printf("输入字符,输入CTRL+D则停止\n");  //输出提示符
  while (fgets(buf, MAXLINE, stdin) != NULL)  //如果从标准输入读数据成功
  {
    if (fputs(buf, stdout) == EOF)   //如果从标准输出发生错误
    {
      printf("字符输出发生错误\n");
      return 1;
    }
  }
  if (ferror(stdin)!= 0)  //如果从标准输入发生错误
  {
    printf("输入出现错误\n");
  }
  return 0;
}
