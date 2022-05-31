#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
#define SERV_PORT 25555   //服务器接听端口号
#define BACKLOG   20      //请求队列中允许请求数
#define BUF_SIZE  256     //缓冲区大小
 
int main(int argc,char *argv[])
{
  int ret;
  time_t tt;
  struct tm *ttm;
  char buf[BUF_SIZE];
  pid_t pid;     //定义管道描述符
  int sockfd;       //定义sock描述符
  int clientfd; //定义数据传输sock描述符
  struct sockaddr_in host_addr;   //本机IP地址和端口信息
  struct sockaddr_in client_addr; //客户端IP地址和端口信息
  int length = sizeof client_addr;
  //创建套接字
  sockfd = socket(AF_INET, SOCK_STREAM, 0); //TCP/IP协议，数据流套接字
  if(sockfd == -1)  //判断socket函数的返回值
  {
    printf("创建socket失败.\n");
    return 0;
  }
  //绑定套接字
  bzero(&host_addr, sizeof host_addr);
  host_addr.sin_family = AF_INET;             //TCP/IP协议
  host_addr.sin_port = htons(SERV_PORT);     //设定端口号
  host_addr.sin_addr.s_addr = INADDR_ANY;       //本地IP地址
  ret = bind(sockfd, (struct sockaddr *)&host_addr, sizeof host_addr); //绑定套接字
  if(ret == -1) //判断bind函数的返回值
  {
    printf("调用bind失败.\n");
    return 1;
  }
  //监听网络端口
  ret = listen(sockfd, BACKLOG);
  if(ret == -1)  //判断listen函数的返回值
  {
    printf("调用listen函数失败.\n");
    return 1;
  }
  while(1)
  {
    clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &length);  //接收接连请求
    if(clientfd == -1)
    {
      printf("调用accept接受连接失败.\n");
      return 1;
    }
    pid = fork();             //创建子进程
    if(pid == 0)              //在子进程是处理
    {            
      while(1)
      {   
        bzero(buf, sizeof buf);  //首先清空缓冲区
        tt = time(NULL);
        ttm = localtime(&tt);    //获取当前时间参数
        strcpy(buf, asctime(ttm));  //将时间信息copy进缓冲区
        send(clientfd, buf, strlen(buf), 0);       //发送数据
        sleep(2);
      }
      close(clientfd);   //调用close函数关闭连接
    }
    else if(pid > 0)
    {
      close(clientfd);  //父进程关闭套接字，准备下一个客户端连接
    }
  }
  return 0;
}
