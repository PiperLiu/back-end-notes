 //判断当前工作路径下某个目录是否存在
 //如果不存在则创建该目录
 //目录名由argv参数传递进去
 #include <fcntl.h>
 #include <sys/types.h>
 #include <dirent.h>
 #include <stdio.h>
 int main(int argc,char *argv[])
 {
   DIR *dp;	    //目录文件指针
   int temp;        //存放mkdir函数的返回值
   if(argc != 2)    //如果参数不正确
   {
     printf("请输入正确的参数！/n");   
     return 1;
   }
   dp = opendir(*(argv+1));        //尝试打开目录
   if(dp == NULL)         //出错，说明目录不存在
   {
     printf("目录不存在！\n");
     temp = mkdir(*(argv + 1),S_IRWXU|S_IRGRP|S_IXOTH);  //创建目录文件
     if(temp == -1)
     {
       printf("创建目录失败！\n");
       return 2;
     }
     else
     {
       printf("创建目录%s成功！\n",*(argv+1));
     } 
   }
   else
   {
      printf("目录%s已经存在！打开成功！\n",*(argv+1));
      closedir(dp);       //关闭目录
   }
   return 0;
 }
 

