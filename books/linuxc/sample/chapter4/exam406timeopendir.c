//使用当前时间的“时+分”信息为名称来创建一个文件夹
//然后在该文件件下以“秒”信息为名称来创建一个文件
//需要判断文件夹和文件是否存在
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc,char *argv)
{
  time_t timetemp;        //定义一个时间结构体变量
  struct tm *p;           //结构体指针
  DIR *dp;                //目录文件指针
  int temp = 0;           //存放mkdir函数的返回值
  int chdirtemp = 0;      //存放chdir函数的返回值
  int fd;                 //文件描述符
  char filetimebuf[3];    //目录时间信息
  char dirtimebuf[5];     //文件时间信息
  char dirnamebuf[10] = "DIR";    //目录名缓冲区
  char filenamebuf[10] = "File";  //文件名缓冲区
  char npath[100];         //当前工作目录的完整路径
  time(&timetemp);          //获得时间参数
  printf("当前时间为%s",asctime(gmtime(&timetemp)));
  p = localtime(&timetemp);
  printf("小时 = %d ,分 = %d ,秒 = %d\n",p->tm_hour,p->tm_min,p->tm_sec);  //输出一次当前的时、分、秒信息
  sprintf(dirtimebuf,"%02d%02d",p->tm_hour,p->tm_min);
  sprintf(filetimebuf,"%02d",p->tm_sec);
  //将时、分秒信息按照2位前端补0的方式格式化送入目录和文件时间buf
  strcat(filenamebuf,filetimebuf);
  strcat(dirnamebuf,dirtimebuf);
  //生成文件和目录名称，存放到对应的缓冲区中
  printf("%s\n",filenamebuf);
  printf("%s\n",dirnamebuf);
  dp = opendir(dirnamebuf); //尝试打开目录
  if(dp == NULL)            //出错，说明目录不存在
  {
    printf("目录%s不存在\n",dirnamebuf);
    temp = mkdir(dirnamebuf,S_IRWXU|S_IRGRP|S_IXOTH);   //尝试创建目录
    if(temp == -1)   //创建目录失败
    {
      printf("创建目录失败。\n");
      return 1;
    }
    else           //创建目录成功
    {
      printf("创建目录%s成功\n",dirnamebuf);
      chdirtemp = chdir(dirnamebuf);         //将当前工作目录切换到新建的目录下
      if(chdirtemp == -1)      //表明切换失败
      {
        printf("切换当前工作目录失败\n");
        return 2;
      } 
      else   //切换当前工作目录成功，创建文件
      {
        if(getcwd(npath,100) == NULL)  //如果已经获得当前的工作目录则打印输出，否则退出
        {
          printf("未能获得当前工作目录路径\n");
          return 3;
        }
        else
        {
          printf("当前工作目录的完整路径是%s\n",npath);
        }
        fd = open(filenamebuf,O_RDWR|O_CREAT,S_IRWXU);   //创建文件
        if(fd != -1)     //表明创建文件成功
        {
          printf("创建文件%s成功\n",filenamebuf);
          close(fd);     //关闭文件
        }     
        else
        {
          printf("创建文件失败\n");
          return 4;
        }
      }
    }
  }
  else    //能打开目录，则表明目录存在
  {
    printf("目录%s已经存在\n",dirnamebuf);
    closedir(dp);       //关闭目录
    //接下来切换当前工作目录到已经存在的目录，创建文件
    chdirtemp = chdir(dirnamebuf);         //将当前工作目录切换到新建的目录下
    if(chdirtemp == -1)      //表明切换失败
    {
      printf("切换当前工作目录失败\n");
      return 2;
    }
    else   //切换当前工作目录成功，创建文件
    {
      if(getcwd(npath,100) == NULL)  //如果已经获得当前的工作目录则打印输出，否则退出
      {
        printf("未能获得当前工作目录路径\n");
        return 3;
      }
      else
      {
        printf("当前工作目录的完整路径是%s\n",npath);
      }
      fd = open(filenamebuf,O_RDWR|O_CREAT,S_IRWXU);   //创建文件
      if(fd != -1)     //表明创建文件成功
      {
        printf("创建文件%s成功\n",filenamebuf);
        close(fd);     //关闭文件
      }     
      else
      {
        printf("创建文件失败\n");
        return 4;
      }
    }
  }
  return 0;
}

