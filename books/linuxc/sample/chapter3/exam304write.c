 //这是一个标准的将字符串写入文件的应用，待创建的文件名由argv[1]给出
 //实例输出创建文件的文件描述符fd和写入文件的字符串长度
 #include <fcntl.h>
 #include <stdio.h>
 int main(int argc,char *argv[])
 {
   int fd;      //文件描述符
   int temp;    //临时变量
   char writebuf[] = "this is a test!\n";    //存放的待写入数据
   if(argc != 2)    //如果参考字符串
   {
     printf("Plz input the correct file name as 'exam304WriteFun filename'\n");
     //输出提示字符串
     return 1;
   }
   else
   {
     fd = open(*(argv+1),O_RDWR|O_CREAT,S_IRWXU); 
    //打开文件，如果没有则创建
   }
   printf("The File Descriptor is %d\n",fd);	//打印文件描述符
   temp = write(fd,writebuf,17);   //使用文件描述符调用文件
   printf("The input length is %d\n",temp);   
   close(fd);
   return 0;
 }

