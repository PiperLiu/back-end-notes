#include <stdio.h>
#include <stdlib.h>
#if defined(MACOS)
#define _IO_UNBUFFERED  __SNBF
#define _IO_LINE_BUF    __SLBF
#define _IO_file_flags  _flags
#define BUFFERSZ(fp)    (fp)->_bf._size
#else
#define BUFFERSZ(fp)    ((fp)->_IO_buf_end - (fp)->_IO_buf_base)
#endif
void    pr_stdio(const char *, FILE *);
int main(void)
{
        FILE    *fp;
        printf("pls enter some str\n");  //提示输入部分字符
        if (getchar() == EOF)   //如果EOF错误
        {
                perror("getchar error");
        }
        fputs("one line to standard error\n", stderr);

        pr_stdio("stdin",  stdin);
        pr_stdio("stdout", stdout);
        pr_stdio("stderr", stderr);   //输出相应的缓冲类型
                perror("fopen error");
        if (getc(fp) == EOF)
                perror("getc error");
        pr_stdio("/etc/motd", fp);
        exit(0);
}
//缓冲输出函数
void  pr_stdio(const char *name, FILE *fp)
{   
        printf("stream = %s, ", name);   //打印缓冲名称
        if (fp->_IO_file_flags & _IO_UNBUFFERED)
                printf("unbuffered");
        else if (fp->_IO_file_flags & _IO_LINE_BUF)
                printf("line buffered");
        else /* if neither of above */
                printf("fully buffered");
        printf(", buffer size = %ld\n", BUFFERSZ(fp));
}

