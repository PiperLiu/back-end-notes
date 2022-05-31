//字符I/O函数getc和putc的应用实例
//实例从标准输入键盘读入字符，然后送到标准输出显示器
#include <stdio.h>
int main(int argc,char *argv)
{
  int temp;   //存放I/O函数的返回值
  printf("输入字符，输入CTRL+D则停止\n");  //输出提示符
  while ((temp = getc(stdin)) != EOF)     //如果没有接收到EOF
  {
    if (putc(temp, stdout) == EOF)           //如果putc函数返回EOF
    {
      printf("字符输出发生错误\n");
      return 1;
    }
  }
  if (ferror(stdin) != 0)    //如果标准输入出现错误
  {
    printf("输入出现错误\n");
    return 2;
  } 
  return 0;
}
