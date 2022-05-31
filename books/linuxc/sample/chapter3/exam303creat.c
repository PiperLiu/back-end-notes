 //使用creat函数来创建文件的应用实例
 //待创建文件的名称由argv参数给出，然后打印文件描述符并且退出
 //调用文件的方式为"./exam303CreatFun 文件名”
 #include <fcntl.h>
 #include <stdio.h>
 int main(int argc,char *argv[])	
 {
   int fd;      //文件描述符
   if (argc!=2) //如果参数不是2个，可执行文件+待创建的文件
   {	
   	printf("Plz input the correct file name as './exam303CreatFun filename'\n"); //参数错误
   	return 1;					//退出
   }
   else
   {
     fd = creat(*(argv+1),S_IRWXU); //参数字符串的第二个数据作为文件名
     printf("The File Descriptor is %d\n",fd);
     return 0;
   }  //可以通过主函数的返回值来判断执行的状态
 }
