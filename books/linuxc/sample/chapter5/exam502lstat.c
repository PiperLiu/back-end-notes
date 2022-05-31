//这是一个使用stat系列函数来获得arvg指定文件属性的实例
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
  int i;
  struct stat buf;     //文件属性存放缓冲区
  char *ptr;
  for (i=1; i<argc;i++)    //命令行参数作为输出参数
  {
    printf("%s 是一个 ", argv[i]);  //首先打印输出文件名
    if (lstat(argv[i], &buf) < 0) 
    //如果lstat函数返回值小于0则表示函数调用失败 
    {
      printf("lstat error");
      continue;              //仅仅退出当前循环
    }
    //以下开始判断文件类型
    if (S_ISREG(buf.st_mode))  
    {
       ptr = "普通文件";     //普通文件
    }
    else if (S_ISDIR(buf.st_mode))
    {
	ptr = "目录文件";  //目录文件
    }
    else if (S_ISCHR(buf.st_mode))
    {
	ptr = "字符设备文件";  //字符设备文件
    }
    else if (S_ISBLK(buf.st_mode))
    {
        ptr = "块设备文件";   //块设备文件
    }
    else if (S_ISFIFO(buf.st_mode))
    {
	ptr = "FIFO";       //先进先出文件
    }
    else if (S_ISLNK(buf.st_mode))
    {
	ptr = "符号链接";   //符号链接文件
    }
    else if (S_ISSOCK(buf.st_mode))
    {
	ptr = "套接字文件";  //套接字文件
    }
    else   //如果不在以上文件类型则表明为未知文件类型
    {
      ptr = "未知文件类型";
    }
    printf("%s\n", ptr);      //输出文件类型
  }
  return 0;
}

