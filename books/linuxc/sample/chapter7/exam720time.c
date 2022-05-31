#include <sys/times.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
//时间统计函数
static void get_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
  static long             clktck = 0;
  if (clktck == 0)        //第一次获得时间
  if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
  {
     perror("调用sysconf函数错误.\n");
  }
  //以下为时间输出
  printf("时钟时间:  %7.2f\n", real / (double) clktck);
  printf("用户CPU时间:  %7.2f\n",(tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);
  printf("系统CPU时间:   %7.2f\n",(tmsend->tms_stime - tmsstart->tms_stime) / (double) clktck);
  printf("子进程时钟时间:  %7.2f\n",(tmsend->tms_cutime - tmsstart->tms_cutime) / (double) clktck);
  printf("子进程系统CPU时间:   %7.2f\n",(tmsend->tms_cstime - tmsstart->tms_cstime) / (double) clktck);
}

//执行并且对cmd命令计时
static void execute_cmd(char *cmd)
{
  struct tms      tmsstart, tmsend;   //时间结构体;
  clock_t         start, end;  //分别存放起始和停止时刻的时钟滴答数
  int             status;      //执行状态
  printf("\n当前执行的命令是: %s\n", cmd);  //输出对应的命令
  if ((start = times(&tmsstart)) == -1)   //获得start时间
  {
     perror("调用times函数出错.\n");
  }
  if ((status = system(cmd)) < 0) //执行命令
  {
     perror("调用system函数出错\n");
  }
  if ((end = times(&tmsend)) == -1) //获得end时间
  {
      perror("调用times函数出错\n");
  }
  get_times(end-start, &tmsstart, &tmsend); //计算运行时间
}
//主函数
int main(int argc, char *argv[])
{
  int  i;
  setbuf(stdout, NULL);  //清空标准输出（屏幕）
  if(argc != 2)  //参数数目错误
  {
    printf("请输入正确的命令.\n");
    exit(0);
  }  
  else
  {
    execute_cmd(argv[1]); //执行命令
    exit(0);
  }
}   

