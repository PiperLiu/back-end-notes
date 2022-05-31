 //这是一个chdir和getcwd函数的应用实例
 //首先使用mkdir函数在当前文件夹下建立一个新的文件夹
 //然后使用chdir函数切换工作目录到新建的文件夹下
 //打印输出切换后的工作路径，然后在该工作目录下建立一个新的文件夹
 #include <stdio.h>
 #include <unistd.h>
 #include <fcntl.h>
 int main(int argc,char *argv[])
 {
    unsigned char temp;
    char npath[200];            //路径字符串缓冲区
    if(argc != 3 )              //如果参数不为3
    {
        perror("请输入正确的参数!\n");      //参数错误
        return 1;                           //退出
    }
    temp = mkdir(*(argv+1),S_IRUSR|S_IWUSR|S_IXUSR);            
    //在当前工作路径（文件夹下）新建一个目录，目录名由arvg[1]指定
    if(temp == -1)      //如果创建失败
    {
      printf("创建文件失败！/n");   //创建目录失败
      return 2;                   //退出
    }
    temp = chdir(*(argv+1));      //切换目录到arvg[1]指定的目录下
    if(temp == -1)               //切换目录失败
    {
      printf("切换目录操作失败！/n");
      return 3;
    }
    else   //切换目录操作成功
    {
      if(getcwd(npath,200) == NULL)     //如果没有获得当前的工作路径
      {
        printf("不能获得当前的工作路径！\n");
        return 4;
      }
      else
      {
        printf("当前的工作路径是 %s\n",npath);  //打印输出当前的工作路径
      }
    }
    temp = mkdir(*(argv+2),S_IRWXU|S_IRGRP|S_IXOTH);  
    //再建立一个由argv[2]指定名称的文件夹
    return 0;
 }
