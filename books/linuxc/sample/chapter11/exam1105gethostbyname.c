#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main(int argc,char *argv[])
{
  char *ptr,**pptr;
  struct hostent *hptr;
  struct in_addr hpaddr;
  //使用argv[1]作为参数
  ptr = argv[1];
  //调用gethostbyname函数，将结果存放到hptr中
  if((hptr = gethostbyname(ptr))== NULL)  //如果调用函数失败
  {
    printf("解析域名%s失败.\n",ptr);
    return 0;
  }
  printf("目标的官方域名是%s\n",hptr->h_name);
  //由于目标可能有多个别名，所以统统打印
  for(pptr = hptr->h_aliases;*pptr != NULL;pptr++)
  {
    printf("目标的别名是%s\n",*pptr);
  }
  //根据地址类型输出地址
  switch(hptr->h_addrtype)
  {
    case AF_INET:
    case AF_INET6:   //针对IPV4和IPV6均进行如下操作，因为之前没有break
    {
      pptr = hptr -> h_addr_list;
      for(;*pptr != NULL;pptr++)
      {
        memcpy(&hpaddr,pptr,4);
        printf("目标地址是:%s\n",inet_ntoa(hpaddr));
      }
    }
    break;  
    default:
      printf("未知的地址类型\n");
  }
  return 0;
}
