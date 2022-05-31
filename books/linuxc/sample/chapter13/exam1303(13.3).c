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
    12  #define INPUTBUFFSIZE  20  //�����������뻺������С
    13  #define QUITBUFFSIZE   20   //�û����뻺������С
    14  #define WRITEBUFFSIZE  50  //д�ļ���������С
    15  #define MAX_COM_NUM    32  //��󴮿���Ŀ
    16  #define HOST_COM_PORT  1   //ʹ��1�Ŵ���
    17
    18  int set_com_config(int fd,int baud_rate,int data_bits,char parity, int stop_bits)
    19  {
    20    //ʡ���˾�����룬�ο���13.1
    21  }
    22
    23  int open_port(int com_port)
    24  {
    25    //ʡ���˾�����룬�ο���13.2
    26  }
    27
    28  //������������������
    29  int main(int argc,char *argv[])
    30  {
    31          int sfd;      //�����ļ�������
    32          int recondfd;   //��¼�ļ�������
    33          char sendbuff[] = "Plz Send Data";     //�����ַ���������
    34          char inputbuff[INPUTBUFFSIZE];        //�����ַ�������
    35          char quitbuff[QUITBUFFSIZE];        //�û����뻺����
    36          char writebuff[WRITEBUFFSIZE];                    //��������д���ļ�������
    37          char enterbuf[3]="\r\n";                      //�س�����buf
    38          int temp,seektemp;                             //ƫ���������м���
    39          struct timezone timez;
    40          time_t timetemp;                           //ʱ��ṹ�����
    41          int j = 0;
    42          int writeCounter = 0;                      //д�������
    43
    44       if((sfd = open_port(HOST_COM_PORT)) < 0) /* �򿪴��� */
    45       {
    46            perror("open_port");    //�򿪴���ʧ��
    47            return 1;
    48       }
    49       if(set_com_config(sfd, 115200, 8, 'N', 1) < 0) /* ���ô��� */
    50       {
    51            perror("set_com_config");   //���ô���ʧ��
    52            return 1;
    53       }
    54      recondfd = get_new_file;   //����һ���µ��ļ����ڼ�¼��������
    55      do
    56       {
    57            printf("System is running(enter 'quit' to exit):");  //�����ʾ��Ϣ���������quit�˳�
    58            write(sfd,sendbuff,strlen(sendbuff));                      //ͨ�����ڷ�������
    59           //�����û����ڻ�ȡ�û�������
    60            memset(quitbuff, 0, QUITBUFFSIZE);
    61            fgets(quitbuff, QUITBUFFSIZE, stdin);   //�Ӽ��̶�ȡ�û��������ŵ�quitbuff��
    62            usleep(50);             //�ȴ�50����ȷ�����������Ѿ���������
    63            memset(inputbuff, 0, INPUTBUFFSIZE);  //Ϊ�����ַ�������������ռ�
    64            if (read(sfd, inputbuff, INPUTBUFFSIZE) > 0)  //�Ӵ��ڶ�ȡ���ݲ�������Ļ�����
    65            {
    66                printf("WindSpeed is: %s", inputbuff);
    67            }
    68            //�����ǽ�����д���¼�ļ�������
    69            time(&timetemp);         //��õ�ǰʱ�����
    70            sprintf(writebuff,"%s",ctime(&timetemp));  //����ǰʱ���������д������
    71            stract(writebuff,inputbuff);  //��ʱ�����ݺͷ�����������
    72            stract(writebuff,enterbuf);  //��ʱ����������������ϻس�����
    73            if(writeCounter == 0)       //��һ��д��
    74            {
    75                  temp = write(recondfd,writebuff,strlen(writebuff));  //д������
    76                  seektemp = lseek(recondfd,0,SEEK_CUR);   //��õ�ǰ��ƫ����
    77                  writeCounter++;   //д�������++
    78            }
    79            else
    80            {
    81                  j = strlen(writebuff) * writeCounter;  //���ƫ����
    82                  seektemp = lseek(recondfd,j,SEEK_SET);
    83                  temp = write(recondfd,writebuff,strlen(writebuff));
    84                  writeCounter++;
    85            }
    86        } while(strncmp(quitbuff, "quit", 4));    //�����鵽�û�����quit���˳�
    87       close(sfd);                    //�رմ���
    88       close(recondfd);         //�رռ�¼�ļ�
    89       return 0;
    90   }
