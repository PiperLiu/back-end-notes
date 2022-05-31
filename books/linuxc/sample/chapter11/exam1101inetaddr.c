#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
int main(int argc,char *argv[])
{
  unsigned long iptemp;
  if(argc != 2)    //如果参数不正确
  {
    printf("请输入正确的ip地址值.\n");
    return 1;      
  }
  iptemp = inet_addr(argv[1]);  //调用inet_addr函数获得网络地址
  printf("返回的ip数值是%lu.\n",iptemp);
  return 0;
}
