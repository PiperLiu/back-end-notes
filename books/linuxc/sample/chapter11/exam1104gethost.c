#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
int main(int argc,char *argv[])
{
  struct hostent *hptr;
  struct in_addr hpaddr;   //定义一个地址结构体  
  if((hptr = gethostbyname(argv[1])) == NULL)
  {
    printf("请输入域名.\n");
    return 1;
  }
  else
  {
    memcpy(&hpaddr,&hptr->h_addr,4);  //拷贝ip地址   
    printf("IP地址为%s.\n",inet_ntoa(hpaddr));
  }
  return 0;
}
