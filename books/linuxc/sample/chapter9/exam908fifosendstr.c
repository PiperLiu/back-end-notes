#include <stdio.h>
#include <stdlib.h>

#define FIFO_FILE "MYFIFO"

int main(int argc, char *argv[])
{
  FILE *fp;
  int i;
  if(argc < 2 ) //如果参数错误
  {
    printf("请使用: %s <pathname>\n",argv[0]);
    exit(1);
  }
  if((fp=fopen(FIFO_FILE,"w"))==NULL) //打开文件
  {
    printf("打开文件失败. \n");
    exit(1);
  }
  for(i=1;i<argc;i++)  //通过管道发送数据
  {
    if(fputs(argv[i],fp)==EOF)
    {
       printf("写fifo失败. \n");
       exit(1);
    }
    if(fputs(" ",fp)==EOF)
    {
      printf("写fifo失败. \n");
      exit(1);
    }
  }
  fclose(fp);
  return 0;
}

