#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
int main(int argc,char *argv[])
{
   int sockfd;          //定义套接口描述符
   if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)  //建立一个socket
   {
      printf ("创建套接字失败.\n");
      return 1;
   }
   else       //socket创建成功
   {
      printf("套接字的ID是:%d\n",sockfd);
   }
   return 0;
}

