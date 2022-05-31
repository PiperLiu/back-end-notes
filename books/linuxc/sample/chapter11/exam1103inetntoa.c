#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
int main(int argc, char *argv[])
{
  struct in_addr addr1,addr2;
  unsigned long netaddr1,netaddr2;
  if(argc != 3)   //如果参数不正确
  {
    printf("请输入正确的参数.\n");
    return 1;                       //退出
  }
  netaddr1 = inet_addr(argv[1]);
  netaddr2 = inet_addr(argv[2]);
  memcpy(&addr1, &netaddr1, 4);
  memcpy(&addr2, &netaddr2, 4);             //拷贝地址
  printf("addr1 = %s : addr2 = %s\n", inet_ntoa(addr1), inet_ntoa(addr2)); //再次输出两个ip地址
  //分别输出IP地址
  printf("%s\n", inet_ntoa(addr1));
  printf("%s\n", inet_ntoa(addr2));
  return 0;
}
