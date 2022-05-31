#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
#define SERV_PORT 25555   //服务器接听端口号
#define BACKLOG       20      //请求队列中允许请求数
#define BUF_SIZE  256     //缓冲区大小
 
int main(int argc, char *argv[])
{
  int ret;   
  time_t tt; 
  struct tm *ttm;
  char buf[BUF_SIZE];  
  int sockfd;       //定义sock描述符   
  struct sockaddr_in serv_addr;   //服务器IP地址和端口信息
  if(argc != 2)
  {
    printf("命令行输入有误\n");       //命令行带IP
    return 1;
  }
  /**创建套接字**/
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);  //TCP/IP协议，数据流套接字
  if(sockfd == -1)  //判断socket函数的返回值
  {
    printf("调用socket函数创建链接失败.\n");
    return 0;
  }
  /**建立连接**/
  bzero(&serv_addr, sizeof serv_addr);
  serv_addr.sin_family = AF_INET;             //TCP/IP协议
  serv_addr.sin_port = htons(SERV_PORT);     //设定端口号   
  //serv_addr.sin_addr.s_addr = INADDR_ANY;     //使用回环地址 127.0.0.1
  inet_aton(argv[1], (struct sockaddr *)&serv_addr.sin_addr.s_addr); //设定IP地址
  while(1)
  {
    bzero(buf, sizeof buf);  //首先清除缓冲区
    tt = time(NULL);
    ttm = localtime(&tt);
    strcpy(buf, asctime(ttm));   //拷贝缓冲区数据
    sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&serv_addr, sizeof serv_addr);    
    //接收数据，然后放入缓冲区
    sleep(2);
  }
  close(sockfd);
  return 0;
}
