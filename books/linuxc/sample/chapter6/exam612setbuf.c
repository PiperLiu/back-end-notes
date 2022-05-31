//流操作的缓冲区设置应用实例
//调用setbuf函数来修改标准输入stdin的缓冲方式
#include <stdio.h>
#include <stdlib.h>
#define SIZE 512                   //定义缓冲区大小
int main(int argc,char *argv[])
{
   char buf[SIZE];                 //缓冲区
   if(setvbuf(stdin, buf, _IONBF, SIZE) != 0)   //将标准输入的缓冲类型设为无缓冲
   {
       perror("将标准输入stdin的输入设置为无缓冲失败!\n");      //如果设置失败
       return 1;
   }
   printf("将标准输入stdin的输入设置为无缓冲成功!\n");
   printf("stdin类型为");                  //打印缓冲区信息
   if(stdin->_flags & _IO_UNBUFFERED)    //判断标准输入流对象的缓冲区类型
   {
      printf("无缓冲\n");
   }
   else if(stdin->_flags & _IO_LINE_BUF)
   {
      printf("行缓冲\n");
   }
   else
   {
      printf("全缓冲\n");
   }
   printf("缓冲区大小为 %ld\n", stdin->_IO_buf_end - stdin->_IO_buf_base);
   //打印缓冲区的大小
   printf("文件描述符为 %d\n", fileno(stdin));  //输出文件描述符
   if(setvbuf(stdin,buf,_IOFBF,SIZE)!=0)
   {
       //将标准输入的缓冲类型设为全缓冲，缓存大小为512
       printf("将标准输入stdin设置为全缓冲失败!\n");
       return 2;     //出错退出
   }
   printf("修改标准输入stdin的类型成功!\n");
   printf("stdin类型为");                    //打印缓冲区信息
   if(stdin->_flags & _IO_UNBUFFERED)  //判断标准输入流对象的缓冲区类型
   {
      printf("无缓冲\n");
   }
   else if(stdin->_flags & _IO_LINE_BUF)
   {
      printf("行缓冲\n");
   }
   else
   {
      printf("全缓冲\n");
   }
   printf("缓冲区大小为%ld\n", stdin->_IO_buf_end - stdin->_IO_buf_base);
   //打印缓冲区的大小
   printf("文件描述符为%d\n", fileno(stdin));  //输出文件描述符
   return 0;
}


