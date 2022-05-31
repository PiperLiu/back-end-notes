 //这是使用argv来传递待打开或者创建文件参数的应用实例
 //打开或者创建文件成功之后打印文件描述符并且关闭文件退出
 //调用文件的格式为“./exam302OpenFun 文件名”
 #include <stdlib.h>
 #include <fcntl.h>
 #include <stdio.h>
 int main(int argc,char *argv[])   //main函数的两个标准参数
 {
   int fd;                //文件描述符
   int temp;              //临时变量
   if(argc != 2)          //如果参数不是两个，则说明用户输入参数错误
   {
     printf("Plz input the correct file name as './exam302OpenFun filename'\n");
     //输出提示格式
   }
   else     //如果格式正确
   {
     fd = open(*(argv+1),O_RDWR|O_CREAT,S_IRWXU);     //创建argv的第二个字符开始指定的文件
     printf("The File Descriptor is %d\n",fd);           //输出文件描述符
     temp = close(fd);    //关闭文件
   } 
   exit(0);             //退出
 }
