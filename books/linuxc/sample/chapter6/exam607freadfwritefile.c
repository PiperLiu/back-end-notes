#include  <stdio.h>
#include  <stdlib.h>
int main(int argc,char *argv[])
{
    FILE *fp1, *fp2;       //流指针
    char buf[1024];        //缓冲区
    int n;      //存放fread和fwrite函数的返回值
    if(argc <=2)    //如果参数错误
    {
       printf("请输入正确的参数\n!");   //参数错误
    }
    if ((fp1 = fopen(*(argv+1), "rb")) == NULL)
    //以只读方式打开源文件，读开始位置为文件开头
    {
         printf("读源文件%s发生错误\n",*(argv+1));
         return 1;      //出错退出
      }
    if ((fp2 = fopen(*(argv+2), "wb")) == NULL)
    //以只写方式打开目标文件，写开始位置为文件结尾
    {
         printf("打开目标文件%s失败\n",*(argv+2));
         return 2;       //出错退出
      }
   //开始复制文件，文件可能很大，缓冲一次装不下，所以使用一个循环进行读写*/
    while ((n = fread(buf, sizeof(char), 1024, fp1)) > 0)
    {
        //读源文件，直到将文件内容全部读完*/
        if (fwrite(buf, sizeof(char), n, fp2) == -1)
        {
            //将读出的内容全部写到目标文件中去
            printf("写如文件发生错误\n");
            return 3;    /*出错退出*/
        }
    }
    printf("从源文件%s读数据写入目标文件%s中完成\n",*(argv+1),*(argv+2)); //输出对应的提示
    if(n == -1)
    {
         //如果因为读入字节小于0而跳出循环，则说明出错了*/
         printf("读文件发生错误\n");
         return 4;     /*出错退出*/
    }
    fclose(fp1);   /*操作完毕，关闭源文件和目标文件*/
    fclose(fp2);
    return 0;
}


