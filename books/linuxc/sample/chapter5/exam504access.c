//对argv传递文件名的文件使用access文件进行权限检测
 #include <fcntl.h>
 #include <stdio.h>
 int main(int argc,char *argv[])
 {
   int temp;
   if(argc != 2)  //参数错误
   {
     printf("文件名参数错误!\n");
     return 1;
   }
   temp = access(*(argv+1),F_OK);  //测试文件是否存在
   if(temp == -1)
   {
     printf("文件不存在!\n");      //文件不存在
     return 2;
   }
   temp = access(*(argv+1),R_OK);  //测试文件是否能读
   if(temp == 0)
   {
     printf("%s文件可以进行读操作!\n",*(argv+1));
   }
   else
   {
     printf("%s文件不允许进行读操作!\n",*(argv+1));		
   }
   temp = access(*(argv+1),W_OK);  //测试文件是否能写
   if(temp == 0)
   {
      printf("%s文件可以进行写操作!\n",*(argv+1));
   }
   else
   {
      printf("%s文件不允许进行写操作!\n",*(argv+1));		
   }
   temp = access(*(argv+1),X_OK);  //测试文件是否可执行
   if(temp == 0)
   {
      printf("%s是一个可执行文件!\n",*(argv+1));
   }
   else
   {
      printf("%s不是一个可执行文件!\n",*(argv+1));		
   }
   return 0;
 }
