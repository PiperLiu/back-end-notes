//判断到底是到了文件的结束还是出错
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  int i;
  FILE *fp;
  if(argc != 2)    //参数错误
  {
    printf("请输入正确的参数\n");
    return 1;
  }
  fp = fopen(*(argv+1),"wb");  
  //打开文件，但是文件为空所以无法读取
  fgetc(fp);                   
  //从文件中读出一个字符，文件为空，所以会报错
  printf("ferror的返回值为%d\n",ferror(fp));   //输出错误信息
  fputs("abcdefgh",fp);   //向文件中写入一些数据
  fclose(fp);             //关闭文件
  fp=fopen(*(argv+1),"r");   //再次打开文件
  fseek(fp,0,SEEK_END);  
  //使用fseek定位到文件末位
  fgetc(fp);    //读入
  if(feof(fp) == 1)   //如果是到了末尾数出
  {
    printf("到达文件结尾\n");
  }
  clearerr(fp); //清除当前错误
  printf("ferror的返回值为%d feof的返回值为%d\n",ferror(fp),feof(fp));  //再次打印错误信息
  fclose(fp);   //关闭文件
  return 0;
}
