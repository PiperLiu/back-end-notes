 //使用gets函数从标准输入（键盘）获得一个以回车换行为结束的字符串，可以带空格
 //运行时候屏幕会提示输入字符处，以回车结尾
 //需要注意的是待输入的字符串存放在writebuf中，不能超过30字节并且不会带回车换行
 #include <fcntl.h>
 #include <stdio.h>
 #include <string.h>
 int main(int argc,char *argv[])
 {
   int fd;      //文件描述符
   int temp;    //临时变量
   char writebuf[30];  //用于存放写入字符串
   if(argc != 2)     //如果参考字符串错误
   {
     printf("Plz input the correct file name as 'exam307WriteFun filename'\n");
     //输出提示字符串
     return 1;
   }
   else
   {
     fd = open(*(argv + 1),O_RDWR|O_CREAT,S_IRWXU); 
    //打开文件，如果没有则创建
   }
   printf("The File Descriptor is %d\n",fd);	//打印文件描述符
   printf("Plz input the strings and use Enter as the end!\n");
   gets(writebuf);	                        //将终端输入的数据写入文件
   temp = write(fd,writebuf,strlen(writebuf));   //使用文件描述符调用文件
   printf("The input length is %d\n",temp);   
   close(fd);
   return 0;
 }
