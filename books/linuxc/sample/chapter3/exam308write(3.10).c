 //这是在上一个实例的基础上用strcat函数解决了回车换行的问题
 #include <fcntl.h>
 #include <stdio.h>
 #include <string.h>
 int main(int argc,char *argv[])
 {
   int fd;      //文件描述符
   int temp;    //临时变量
   char writebuf[30];  //用于存放写入字符串
   char endbuf[] = "\n";  //存放一个回车换行符号
   if(argc != 2)     //如果参考字符串错误
   {
     printf("Plz input the correct file name as 'exam308WriteFun filename'\n");
     //输出提示字符串
     return 1;
   }
   else
   {
     fd = open(*(argv + 1),O_RDWR|O_CREAT,S_IRWXU); 
    //打开文件，如果没有则创建
   }
   printf("The File Descriptor is %d\n",fd);	//打印文件描述符
   printf("Plz input the strings!\n");
   gets(writebuf);	                        //将终端输入的数据写入文件
   strcat(writebuf,endbuf);			//添加换行符
   temp = write(fd,writebuf,strlen(writebuf));   //使用文件描述符调用文件
   printf("The input length is %d\n",temp);   
   close(fd);
   return 0;
 }
