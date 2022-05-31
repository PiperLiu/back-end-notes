#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
int main(int argc,char *argv[])
{
  int temp;
  struct in_addr *testaddr;  //定义一个结构体
  if(argc != 2)
  {
    printf("请输入正确的ip地址.\n");
    return 1;
  }
  temp = inet_aton(argv[1],testaddr);
  if(temp == 0)
  {
    printf("调用inet_aton失败.\n"); 
    return 1;
  }
  else
  {
    printf("转换后的ip地址是%lu.\n",testaddr->s_addr);
  }
  return 0;
}
