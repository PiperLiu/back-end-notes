1  #include <stdio.h>
     2  #include <stdlib.h>
     3  #include <string.h>
     4  #include <sys/types.h>
     5  #include <sys/stat.h>
     6  #include <errno.h>
     7  #include <unistd.h>
     8  #include <sys/time.h>
     9  #include <fcntl.h>
    10  #include <termios.h>
    11
    12  #define INPUTBUFFSIZE  20  //风力数据输入缓冲区大小
    13  #define QUITBUFFSIZE   20   //用户输入缓冲区大小
    14  #define WRITEBUFFSIZE  50  //写文件缓冲区大小
    15  #define MAX_COM_NUM    32  //最大串口数目
    16  #define HOST_COM_PORT  1   //使用1号串口
    17
    18  int set_com_config(int fd,int baud_rate,int data_bits,char parity, int stop_bits)
    19  {
    20    //省略了具体代码，参考例13.1
    21  }
    22
    23  int open_port(int com_port)
    24  {
    25    //省略了具体代码，参考例13.2
    26  }
    27
    28  //以下是主函数的内容
    29  int main(int argc,char *argv[])
    30  {
    31          int sfd;      //串口文件描述符
    32          int recondfd;   //记录文件描述符
    33          char sendbuff[] = "Plz Send Data";     //发送字符串缓冲区
    34          char inputbuff[INPUTBUFFSIZE];        //接收字符缓冲区
    35          char quitbuff[QUITBUFFSIZE];        //用户输入缓冲区
    36          char writebuff[WRITEBUFFSIZE];                    //风力数据写入文件缓冲区
    37          char enterbuf[3]="\r\n";                      //回车换行buf
    38          int temp,seektemp;                             //偏移量计算中间量
    39          struct timezone timez;
    40          time_t timetemp;                           //时间结构体变量
    41          int j = 0;
    42          int writeCounter = 0;                      //写入计数器
    43
    44       if((sfd = open_port(HOST_COM_PORT)) < 0) /* 打开串口 */
    45       {
    46            perror("open_port");    //打开串口失败
    47            return 1;
    48       }
    49       if(set_com_config(sfd, 115200, 8, 'N', 1) < 0) /* 配置串口 */
    50       {
    51            perror("set_com_config");   //配置串口失败
    52            return 1;
    53       }
    54      recondfd = get_new_file;   //创建一个新的文件用于记录风力数据
    55      do
    56       {
    57            printf("System is running(enter 'quit' to exit):");  //输出提示信息，如果输入quit退出
    58            write(sfd,sendbuff,strlen(sendbuff));                      //通过串口发送命令
    59           //以下用户用于获取用户的输入
    60            memset(quitbuff, 0, QUITBUFFSIZE);
    61            fgets(quitbuff, QUITBUFFSIZE, stdin);   //从键盘读取用户的输入存放到quitbuff中
    62            usleep(50);             //等待50毫秒确定串口数据已经反馈回来
    63            memset(inputbuff, 0, INPUTBUFFSIZE);  //为接收字符串缓冲区分配空间
    64            if (read(sfd, inputbuff, INPUTBUFFSIZE) > 0)  //从串口读取数据并且在屏幕上输出
    65            {
    66                printf("WindSpeed is: %s", inputbuff);
    67            }
    68            //以下是将数据写入记录文件的内容
    69            time(&timetemp);         //获得当前时间参数
    70            sprintf(writebuff,"%s",ctime(&timetemp));  //将当前时间参数放入写缓冲区
    71            stract(writebuff,inputbuff);  //将时间数据和风力数据连接
    72            stract(writebuff,enterbuf);  //在时间风力数据最后添加上回车换行
    73            if(writeCounter == 0)       //第一次写入
    74            {
    75                  temp = write(recondfd,writebuff,strlen(writebuff));  //写入数据
    76                  seektemp = lseek(recondfd,0,SEEK_CUR);   //获得当前的偏移量
    77                  writeCounter++;   //写入计数器++
    78            }
    79            else
    80            {
    81                  j = strlen(writebuff) * writeCounter;  //获得偏移量
    82                  seektemp = lseek(recondfd,j,SEEK_SET);
    83                  temp = write(recondfd,writebuff,strlen(writebuff));
    84                  writeCounter++;
    85            }
    86        } while(strncmp(quitbuff, "quit", 4));    //如果检查到用户输入quit则退出
    87       close(sfd);                    //关闭串口
    88       close(recondfd);         //关闭记录文件
    89       return 0;
    90   }
