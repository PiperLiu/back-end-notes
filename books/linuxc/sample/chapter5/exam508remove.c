#include <stdio.h>
int main(int argc,char *argv[])
{
  int ret;
  if(argc != 2)    //如果参数错误
  {
    printf("请输入待删除的文件名称！\n");
    return 1;
  }
  ret = remove(*(argv+1));    //删除文件
  if(ret == 0)                //删除文件成功
  {
    printf("删除文件%s成功！\n",*(argv+1));
  }
  return 0;
}

