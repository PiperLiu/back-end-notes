//这是一个在参数指定文件中连续写入当前时间的应用
//文件以1秒为时间间隔，将当前的时间写入文件，然后回车换行
 //这是一个使用lseek在一个文件中连续写入字符串的应用
 #include <fcntl.h>
 #include <stdio.h>
 #include <string.h>
 #include <sys/time.h>
 int main(int argc,char *argv[])
 {
   time_t timetemp;
   struct tm *p;
   int temp,seektemp;                         //偏移量计算中间量
   int fd;                                    //文件描述符
   char writebuf[50];                        //写字符串缓冲区
   char filenamebuf[10] = "File";             //文件名称缓冲区
   char timebuf[6];                          //时间缓冲区
   struct timeval timenow,timeold;            //时间变量
   struct timezone timez;
   //time_t timetemp;                           //时间结构体变量
   //struct tm *p;
   int j = 0;
   int writeCounter = 0;                      //写入计数器

        time(&timetemp);          //获得时间参数
        printf("%s",asctime(gmtime(&timetemp)));
        p = localtime(&timetemp);
        printf("%d:%d:%d\n",p->tm_hour,p->tm_min,p->tm_sec);

   gettimeofday(&timeold,&timez);             //取得一个时间信息作为参考时间信息
   if(argc!= 2)                               //如果参数错误
   {
   	 printf("Plz input the corrcet file name as './exam39lseekFun filename string'!\n");
   	 return 1;					//如果参数不正确则退出
   }
   fd = open(*(argv+1),O_RDWR|O_CREAT,S_IRWXU); //打开文件如果没有则创建
   while(1)   //进入主循环
   {
     while(1)   //1毫秒延时判断
     {
       gettimeofday(&timenow,&timez);    //获取当前时间参数
       if((timenow.tv_sec - timeold.tv_sec) == 1)  //如果到达一秒
       {
          timeold = timenow;    //更新保存的时间信息
          break;                //1秒时间到，退出
       }
     }
     time(&timetemp);         //获得当前时间参数
 /*    p = localtime(&timetemp);
     sprintf(timebuf,"%02d%02d%02d",p->tm_hour,p->tm_min,p->tm_sec);
     if( p->tm_sec == 0)  //如果是分钟开始
     {
       //p = localtime(&timetemp);
      //sprintf(timebuf,"%02d%02d%02d",timeold.tv_hour,timeold.tv_min,timeold.tv_sec);
       strcat(filenamebuf,timebuf);
       fd = open(filenamebuf,O_RDWR|O_CREAT,S_IRWXU);   //创建一个文件
     }
     //time(&timetemp);                    //获得当前的时间参数
*/ 
    sprintf(writebuf,"%s",ctime(&timetemp));     //将当前时间参数放入写缓冲区
     printf("%s",&writebuf);            //在屏幕上打印单签writebuf的内容
     if(writeCounter == 0)		//第一次写入
     {
       temp = write(fd,writebuf,strlen(writebuf));  //写入数据
       seektemp = lseek(fd,0,SEEK_CUR);   //获得当前的偏移量
       writeCounter++;   //写入计数器++
     }
     else
     {
       j = strlen(writebuf) * writeCounter;  //获得偏移量
       seektemp = lseek(fd,j,SEEK_SET);
       temp = write(fd,writebuf,strlen(writebuf));
       writeCounter++;
    }
   }
  close(fd);
  return 0;
}
