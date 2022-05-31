//将一个格式化的字符串写入文件
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
  FILE *fp;        //流文件结构指针
  int h,m,s;      //时分秒信息
  int temp;       //存放fprintf的返回值
  int i;
  if(argc != 2)   //文件参数错误
  {
    printf("文件参数错误\n");
    return 1;
  }
  srand((int)time(0));  //调用srand函数对随机数函数rand的种子进行初始化
  fp = fopen(*(argv+1),"a+b");   //读写方式打开文件
  for(i = 0;i<10;i++)
  {
    h = 1 + (int)(10.0 * rand()/RAND_MAX + 1.0); 
    m = 1 + (int)(10.0 * rand()/RAND_MAX + 1.0);
    s = 1 + (int)(10.0 * rand()/RAND_MAX + 1.0);
    //分别产生3个位于1~10的随机数
    temp = fprintf(fp,"%02d%02d%02d\n",h,m,s);   //打印字符串到fp中
    if(temp < 0)     //打印出错
    {
      printf("第%d次将字符串打印到%s文件中失败\n",i,*(argv+1));
      return 2;
    }
    else
    {
      printf("第%d次将%d个字符打印到%s文件成功\n",i,temp,*(argv+1));
    }
  }
  fclose(fp);   //关闭流
  return 0;
}
