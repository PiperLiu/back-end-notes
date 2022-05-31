//这是一个使用fseek函数来定位流文件中的位置
//然后讲一个字符串连续写入文件的应用
 #include <stdio.h>
 int main(int argc,char *argv[])
 {
   int temp,seektemp,i,j;
   FILE *fp;                 //流结构文件指针
   char writebuf[] = "this is a test!\n";   //写缓冲区
   if(argc!= 2)       //如果参数错误
   {
     printf("请输入正确的参数!\n");
     return 1;       //如果参数不正确则退出
   }
   fp = fopen(*(argv+1),"a+b");     //打开文件
   for(i=0;i<10;i++)
   {
     j = sizeof(writebuf) * (i+1); //计算下一次的偏移量
     fseek(fp,j,SEEK_SET);
     temp = fputs(writebuf,fp);    //写入数据，没有进行出错处理
   }
   fclose(fp);
   return 0;
 } 
