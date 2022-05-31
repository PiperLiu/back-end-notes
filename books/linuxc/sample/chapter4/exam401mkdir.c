 //这是一个使用mkdir来创建目录的应用实例
 //目录的名称由argv给出
 #include <fcntl.h>
 #include <stdio.h>
 int main(int argc,char *argv[])
 {
   int temp;
   if(argc!=2)    //如果参数格式不正确
   {
     printf("文件参数错误!\n");
     return 1;		//退出
   }
   temp = mkdir(*(argv+1),S_IRWXU|S_IRGRP|S_IXOTH); //必须最少指定一个执行权限位
   if(temp == -1)       //如果创建目录失败
   {
     printf("创建目录失败\n");
     return 2;	//退出
   }
   return 0;
 }
