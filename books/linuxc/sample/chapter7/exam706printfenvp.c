/*这是一个输出envp环境变量的实例*/
#include <stdio.h>
int main(int argc, char *argv[ ], char *envp[ ])
{
   printf("这是参数argc\n%d\n", argc);  //首先打印参数的数目
   printf("这是参数argv\n");  //以下打印参数列表
   while(*argv)  //如果不为空，则输出这些字符串
   {
       printf("%s\n", *(argv++));
   }
   printf("这是环境变量envp\n");  //以下是envp字符串参数
   while(*envp)   //输出envp参数
   {
       printf("%s\n", *(envp++));
   }
    return 0;
}
