#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define  PORT 5555       //定义端口号
int main(int argc,char *argv[])
{
   int sockfd;            //定义套接口描述符
   struct sockaddr_in addr;     //定义IPv4套接口地址数据结构addr
   int addr_len = sizeof(struct sockaddr_in);
   if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)  //建立一个socket
   {
      printf("创建套接字失败!\n");
      return 1;
   }
   bzero(&addr,sizeof(struct sockaddr_in));     //清空表示地址的结构体变量
   addr.sin_family = AF_INET;               //设置addr的成员信息
   addr.sin_port = htons(PORT);
   addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址设为本机IP
   if(bind(sockfd, (struct sockaddr *)(&addr), sizeof(struct sockaddr))<0)
   {
      printf("绑定端口失败!");
      return 1;
   }
   return 0;
}

