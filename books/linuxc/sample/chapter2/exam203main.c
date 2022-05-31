#include <stdio.h>
int main(int argc,char *argv[])   //第一个存放参数的个数，第二个缓冲区存放参数
{
        unsigned int i=0;
        printf("%d\n",argc);
        for(i=0;i<argc;i++)
        {
                printf("%s\n",argv[i]);
        }
        return 0;
}

