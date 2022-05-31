 1  #include<termios.h>
     2  #define MAX_COM_NUM    32  //��󴮿���Ŀ
     3
     4  int open_port(int com_port)
     5  {
     6       int fd;
     7  #if (COM_TYPE == GNR_COM)  /* ʹ����ͨ���� */
     8       char *dev[] = {"/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2"};
     9  #else /* ʹ��USBת���� */
    10       char *dev[] = {"/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2"};
    11  #endif
    12       if ((com_port < 0) || (com_port > MAX_COM_NUM))
    13       {
    14            return -1;
    15       }
    16       /* �򿪴��� */
    17       fd = open(dev[com_port - 1], O_RDWR|O_NOCTTY|O_NDELAY);
    18       if (fd < 0)
    19       {
    20                perror("open serial port");
    21                return(-1);
    22       }
    23
    24       /*�ָ�����Ϊ����״̬*/
    25       if (fcntl(fd, F_SETFL, 0) < 0)
    26       {
    27            perror("fcntl F_SETFL\n");
    28       }
    29
    30       /*�����Ƿ�Ϊ�ն��豸*/
    31       if (isatty(STDIN_FILENO) == 0)
    32       {
    33            perror("standard input is not a terminal device");
    34       }
    35       return fd;
    36  }
