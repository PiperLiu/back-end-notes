 //这是一个利用fgetpos来将一个字符串写入文件的实例
 //在每次写入文件成功之后将当前文件偏移量给出
#include <stdio.h>
#include <stdlib.h> 
int main(int argc,char *argv[])
 {
   int temp,seektemp,i,j;
   FILE *fp;            //流文件结构指针
   fpos_t ps;           //当前偏移量指针
   char writebuf[] = "this is a test!\n"; //写缓冲区
   if(argc!= 2)        //如果参数不正确
   {
     printf("请输入正确的参数!\n");
     return 1;         //如果参数不正确则退出
   }
   fp = fopen(*(argv+1),"a+b");     //打开文件
   for(i=0;i<10;i++)
   {
     j = sizeof(writebuf) * (i+1);      //计算下一次的偏移量
     fseek(fp,j,SEEK_SET);
     temp = fputs(writebuf,fp);         //写入数据
     fgetpos(fp,&ps);               //获得当前的偏移量
     printf("当前文件偏移量fpos为%ld \n",ps);   //打印当前的偏移量输出
   }
   fclose(fp);
   return 0;
 }
