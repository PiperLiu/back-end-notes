//这是一个使用chmod函数来修改文件的权限的实例
//文件名使用argv参数传递
#include <fcntl.h>
#include <stdio.h>
int main(int argc,char *argv[])
{
    int ret;
    struct stat	statbuf;   //文件状态缓冲区
    if(argc != 3)          //如果参数格式错误直接错误
    {
       printf("请输入正确的2个文件名！\n");
       return 1;  //直接退出
    }
    ret = stat(*(argv+1),&statbuf);    //获得文件的属性
    if (ret< 0)                        //取文件属性失败
    {
      printf("没有取得文件对应的属性!\n");
    }
    else
    {
      if(chmod(*(argv+1), (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)  //修改参数1对应的文件权限
      {
         printf("修改文件%s权限出错",*(argv+1));      
      }
    }
    if (chmod(*(argv+2), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)  //修改参数2对应文件权限
    {
       printf("修改文件%s权限出错",*(argv+2));
    }
    return 0;
}
