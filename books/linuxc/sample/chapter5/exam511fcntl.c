//这是一个使用fcntl函数来对文件描述符进行操作实例
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
   int val;
   if (argc != 2)   //如果参数错误
   {
     printf("请输入正确的参数!\n");  
   }
   if ((val = fcntl((atoi(*argv+1)), F_GETFL, 0)) < 0)
   {
     printf("使用fcntl操作文件描述符错误%d", atoi(*(argv+1)));
   }
   switch (val & O_ACCMODE)      //判断文件的类型 
   {
     case O_RDONLY:     
	printf("只读\n");
	break;
     case O_WRONLY:   
	printf("只写\n");  
	break;
     case O_RDWR:    
	printf("读写\n");
	break;
     default:
	printf("未知的模式\n");
   }
   if (val & O_APPEND)  
   {
     printf(",写时候追加\n");
   }
   if (val & O_NONBLOCK) 
   {
     printf(",非阻塞\n");
   }
#if defined(O_SYNC)
   if (val & O_SYNC)   //等待数据和属性写完成
   {
     printf(",同步写\n");
   }
#endif
#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC)
   if (val & O_FSYNC)  //等待写完成
   {
      printf(", 等待写完成");
   }
#endif
   putchar('\n');
   return 0;
}


