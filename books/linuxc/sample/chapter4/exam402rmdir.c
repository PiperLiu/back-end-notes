 //使用rmdir函数删除一个目录
 //目录的名称由argv传递
 #include <fcntl.h>
 #include <stdio.h>
 int main(int argc,char *argv[])
 {
   int temp;
   if(argc != 2)  //如果参数错误
   {
     printf("请输入正确的参数!\n");
     return 1;		//参数错误，退出
   }
   temp = rmdir(*(argv+1));  //删除目录文件
   if(temp == 0)
   {
      printf("删除目录%s成功\n",*(argv+1));  //删除完成
   }
   else
   {
   	  printf("删除目录%s失败\n",*(argv+1));  //删除失败
   }	
   return 0;
 }


