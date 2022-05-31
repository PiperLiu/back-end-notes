//这是一个将argv1给出的文件名称修改为argv2字符串的实例
#include <fcntl.h>
#include <stdio.h>
int main(int argc,char *argv[])
{
  int temp;
  if(argc != 3)	//如果不是三个参数，则报错
  {
    printf("文件参数错误！\n");  
    return 1;
  }
  temp = rename(*(argv+1),*(argv+2));  //将前者修改为后者
  if(temp == -1)		       //如果修改文件出错
  {
    printf("修改%s文件名失败！\n",*(argv+1));			//改名出错
  }
  else
  {
    printf("将文件%s名称修改为%s成功！\n",*(argv+1),*(argv+2));
  }
  return 0;	//推出
}
