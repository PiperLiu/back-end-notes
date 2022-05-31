//使用umask函数来修改文件属性关键字，并且创建两个测试文件
#include <fcntl.h>
#include <stdio.h>
#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
//定义操作权限字符串
int main(void)
{
    umask(0);    //原始默认权限
    if (creat("umasktest1", RWRWRW) < 0)    //创建文件umasktest1
    {
      printf("创建测试文件1失败\n");
    }
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    //修改文件创建关键字
   if (creat("umasktest2", RWRWRW) < 0)   //创建文件umasktest2
   {
      printf("创建测试文件2失败\n");
   }
   return 0;
}
