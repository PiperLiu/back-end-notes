 //这是一个使用lseek在一个文件中连续写入用户输入字符串的应用
 //用户输入字符串由gets函数从键盘输入获取
 //这个地方必须用strlen函数而不是sizeof，前者是缓冲区实际大小而后者是缓冲区大小
 #include <fcntl.h>
 #include <stdio.h>
 #include <string.h>
 int main(int argc,char *argv[])
 {
   int temp,seektemp,i,j;
   int fd;                                    //文件描述符
   char writebuf[30];                 //用于存放待写入数据，最长30字节
   char endbuf[] = "\r\n";            //用于存放回车换行
   if(argc!= 2)                               //如果参数错误
   {
   	 printf("Plz input the corrcet file name as './exam310lseekFun filename string'!\n");
   	 return 1;					//如果参数不正确则退出
   }
   fd = open(*(argv+1),O_RDWR|O_CREAT,S_IRWXU); //打开文件如果没有则创建
   printf("Plz input the string and use Enter as the end!\n");  //提示输入数据
   gets(writebuf);				//获得字符串
   strcat(writebuf,endbuf);			//连接回车换行
   temp = write(fd,writebuf,strlen(writebuf));	//写入数据
   seektemp = lseek(fd,0,SEEK_CUR);             //获得当前的偏移量  
   for(i=0;i<10;i++)                            //连续写入10个字符串
   {
     printf("%d,Plz input the string and use Enter as the end!\n",i);  //提示输入数据
     gets(writebuf);                              //获得字符串
     strcat(writebuf,endbuf);                     //连接回车换行    
     j = sizeof(writebuf) * (i+1);             //计算下一次的偏移量
     seektemp = lseek(fd,j,SEEK_SET);
     temp = write(fd,writebuf,strlen(writebuf));   //写入数据
   }
   close(fd);                              //关闭文件
   return 0;
 }
