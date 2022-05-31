#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main(void)
{
  int status;  //存放system函数的返回值
  printf("system函数调用date命令.\n");
  status = system("date");         //调用date命令获得时间相关信息
  if(status <0)    //如果status小于0，表明调用出错 
  {
    perror("system函数调用date出现错误.\n");  //调用出错
    exit(0);
  }
  printf("system函数的退出值是%d \n",status);  //输出system函数的返回值
  printf("system函数调用nosuchcommand命令.\n");
  if((status=system("nosuchcommand"))<0) //如果status为nosuchcommand的返回值
  {
    printf("system函数调用nosuchcommand错误");
    exit(0);
  }
  printf("system函数的退出值是%d \n",status); //打印对应状态
  printf("system函数调用who命令\n");
  if((status=system("who; exit 44"))<0) //调用who函数
  {
    perror("system函数调用who出现错误");  //打印错误信息
    exit(0);
  }
  printf("system函数的退出值是%d\n",status);  //打印退出状态
  exit(0);
}
