#include<unistd.h>
int main(int argc,char *argv[])
{
  char *arg[] = {"ls","-al","/etc/passwd",(char*)0};  //将参数放到一个数组中，然后传递
  execv("/bin/ls",arg);  //执行ls命令，参数由argv数组传递
  return 0;
}
