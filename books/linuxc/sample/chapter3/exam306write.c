 //应用实例使用了argv[2]来传送待写入数据，但是不能有空格
 //输入格式为exam306WriteFun + 文件名称 + 待写入字符串
 #include <fcntl.h>
 #include <stdio.h>
 #include <string.h>
 int main(int argc,char *argv[])
 {
   int fd;      //文件描述符
   int temp;    //临时变量
   if(argc != 3)    //如果参考字符串错误
   {
     printf("Plz input the correct file name as 'exam306WriteFun filename string'\n");
     //输出提示字符串
     return 1;
   }
   else
   {
     fd = open(*(argv + 1),O_RDWR|O_CREAT,S_IRWXU); 
    //打开文件，如果没有则创建
   }
   printf("The File Descriptor is %d\n",fd);	//打印文件描述符
   temp = write(fd,*(argv + 2),strlen(*(argv+2)));   //使用文件描述符调用文件
   printf("The input length is %d\n",temp);   
   close(fd);
   return 0;
 }
