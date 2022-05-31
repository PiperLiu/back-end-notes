#include <string.h>
#include <stdio.h>
#include <errno.h>
int main(int argc, char *argv[])
{
        fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
        errno = ENOSPC;   //传递错误标号
        perror(argv[0]);     //打印出错应用代码
        return 0;
}

