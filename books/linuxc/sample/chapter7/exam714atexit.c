#include <stdio.h>
#include <stdlib.h>
//用于登记退出执行的第一个函数
void exitfun1(void)
{
  printf("这是第一个终止处理函数!\n");
  return;
}
//用于登记执行的第二个函数
void exitfun2(void)
{
  printf("这是第二个终止处理函数!\n");
  return;
}
int main(int argc,char *argv[])
{
  atexit(exitfun1);  //登记两个函数
  atexit(exitfun2); 
  atexit(exitfun2);  //再次登记
  printf("这是主程序的输出!\n"); //在主程序中输出一个字符串
  exit(0);
}
