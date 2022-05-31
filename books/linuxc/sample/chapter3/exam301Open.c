 //这是一个标准的open函数调用实例，打开文件opentest，如果没有则创建
 //并且返回文件的描述符，并且关闭文件后退出
 #include <stdlib.h>
 #include <fcntl.h>
 #include <stdio.h>
 int main(void)
 {
   int fd;                //文件描述符
   int temp;              //临时变量
   fd = open("./opentest",O_RDWR|O_CREAT,S_IRWXU);  //创建文件examOpenFun
   printf("The File Descriptor is %d\n",fd);           //输出文件描述符
   temp = close(fd);    //关闭文件
   exit(0);             //退出
 }

