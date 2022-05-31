//这是一个分别打印三个标准流和一个文件流
//的缓冲方式的应用实例
#include <stdio.h>
#include <stdlib.h>

#if defined(MACOS)
#define _IO_UNBUFFERED  __SNBF
#define _IO_LINE_BUF    __SLBF
#define _IO_file_flags  _flags
#define BUFFERSZ(fp)    (fp)->_bf._size
#else
#define BUFFERSZ(fp)    ((fp)->_IO_buf_end - (fp)->_IO_buf_base)
#endif
//以上是关于缓冲方式和缓冲区大小的预定义
void pr_stdio(const char *, FILE *);
//子函数声明
int main(int argc,char *argv[])
{
  FILE  *fp;   //流文件结构指针
  pr_stdio("stdin",  stdin);    //标准输入
  pr_stdio("stdout", stdout);   //标准输出
  pr_stdio("stderr", stderr);   //标准出错处理
  printf("fopen error");
  if (getc(fp) == EOF)
  {
    printf("getc error\n");
  }      
  return 0;
}
//测试缓冲输出函数
void  pr_stdio(const char *name, FILE *fp)
{   
  printf("当前流是%s, ", name);   //打印流的名称
  if (fp->_IO_file_flags & _IO_UNBUFFERED)
  {
    printf("无缓冲\n");
  }
  else if (fp->_IO_file_flags & _IO_LINE_BUF)
  {
    printf("行缓冲\n");
  }
  else
  { 
    printf("全缓冲\n");
  }
  printf(", 缓冲区大小 = %ld\n", BUFFERSZ(fp));
  return;
}
