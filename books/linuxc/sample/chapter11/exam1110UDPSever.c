#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
#define SERV_PORT 25555   //服务器接听端口号
#define BACKLOG    20      //请求队列中允许请求数
#define BUF_SIZE  256     //缓冲区大小
 
int main(int argc,char *argv[])
{
    int ret;
    char buf[BUF_SIZE];
    pid_t pid;     //定义管道描述符 
    int sockfd;       //定义sock描述符
    int clientfd; //定义数据传输sock描述符
    struct sockaddr_in host_addr;   //本机IP地址和端口信息
    struct sockaddr_in client_addr; //客户端IP地址和端口信息
    int length = sizeof client_addr;
    //创建套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  //TCP/IP协议，数据流套接字
    if(sockfd == -1) //判断socket函数返回值 
    {
        printf("创建socket失败.\n"); 
        return 1;
    } 
    /**绑定套接字**/ 
    bzero(&host_addr, sizeof host_addr);
    host_addr.sin_family = AF_INET;             //TCP/IP协议 
    host_addr.sin_port = htons(SERV_PORT);     //设定端口号
    host_addr.sin_addr.s_addr = INADDR_ANY;       //本地IP地址
    ret = bind(sockfd, (struct sockaddr *)&host_addr, sizeof host_addr); //绑定套接字
    if(ret == -1)   //判断bind函数返回值
    {
        printf("调用bind函数失败.\n"); 
        return 1;
    }       
    while(1)
    {
        bzero(buf, sizeof buf);
        ret = recvfrom(sockfd, buf, sizeof(buf), 0 ,(struct sockaddr *)&client_addr, &length);
        //接收接连请求 
        if(ret == -1)   //判断recvfrom函数的返回值
        {
            printf("接受连接成功");
            return 1; 
        } 
       // printf("Client IP:%s \n", inet_ntoa(client_addr.sin_addr.s_addr)); //输出客户端IP          
        printf("接收到: %s\n", buf);      
        sleep(2);      
    } 
    close(clientfd);   //关闭连接
    return 0; 
}
