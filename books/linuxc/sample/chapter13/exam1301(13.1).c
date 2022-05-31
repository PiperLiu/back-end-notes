1  #include <termios.h>
     2
     3  int set_com_config(int fd,int baud_rate,
     4                         int data_bits, char parity, int stop_bits)
     5  {
     6       struct termios new_cfg,old_cfg;
     7       int speed;
     8
     9       /*保存并测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/
    10       if  (tcgetattr(fd, &old_cfg)  !=  0)
    11       {
    12            perror("tcgetattr");
    13            return -1;
    14       }
    15       /* 设置字符大小*/
    16       new_cfg = old_cfg;
    17       cfmakeraw(&new_cfg); /* 配置为原始模式 */
    18       new_cfg.c_cflag &= ~CSIZE;
    19       /*设置波特率*/
    20       switch (baud_rate)
    21       {
    22            case 2400:
    23            {
    24                 speed = B2400;
    25            }
    26            break;
    27            case 4800:
    28            {
    29                 speed = B4800;
    30            }
    31            break;
    32            case 9600:
    33           {
    34                 speed = B9600;
    35            }
    36            break;
    37            case 19200:
    38            {
    39               speed = B19200;
    40            }
    41            break;
    42            case 38400:
    43            {
    44                speed = B38400;
    45            }
    46            break;
    47
    48            default:
    49            case 115200:
    50            {
    51                speed = B115200;
    52            }
    53            break;
    54       }
    55       cfsetispeed(&new_cfg, speed);
    56       cfsetospeed(&new_cfg, speed);
    57
    58       /*设置停止位*/
    59       switch (data_bits)
    60       {
    61            case 7:
    62            {
    63                  new_cfg.c_cflag |= CS7;
    64            }
    65            break;
    66
    67            default:
    68            case 8:
    69            {
    70                  new_cfg.c_cflag |= CS8;
    71            }
    72            break;
    73       }
    74       /*设置奇偶校验位*/
    75       switch (parity)
    76       {
    77            default:
    78            case 'n':
    79            case 'N':
    80            {
    81                 new_cfg.c_cflag &= ~PARENB;
    82                 new_cfg.c_iflag &= ~INPCK;
    83            }
    84            break;
    85
    86            case 'o':
    87            case 'O':
    88            {
    89                new_cfg.c_cflag |= (PARODD | PARENB);
    90                new_cfg.c_iflag |= INPCK;
    91            }
    92            break;
    93
    94            case 'e':
    95            case 'E':
    96            {
    97                new_cfg.c_cflag |= PARENB;
    98                new_cfg.c_cflag &= ~PARODD;
    99                new_cfg.c_iflag |= INPCK;
   100            }
   101            break;
   102
   103            case 's':  /*as no parity*/
   104            case 'S':
   105            {
   106            new_cfg.c_cflag &= ~PARENB;
   107            new_cfg.c_cflag &= ~CSTOPB;
   108            }
   109            break;
   110       }
   111
   112       /*设置停止位*/
   113       switch (stop_bits)
   114       {
   115            default:
   116            case 1:
   117            {
   118                new_cfg.c_cflag &=  ~CSTOPB;
   119            }
   120            break;
   121
   122            case 2:
   123            {
   124                new_cfg.c_cflag |= CSTOPB;
   125            }
   126       }
   127
   128       /*设置等待时间和最小接收字符*/
   129       new_cfg.c_cc[VTIME]  = 0;
   130       new_cfg.c_cc[VMIN] = 1;
   131
   132       /*处理未接收字符*/
   133       tcflush(fd, TCIFLUSH);
   134       /*激活新配置*/
   135       if ((tcsetattr(fd, TCSANOW, &new_cfg)) != 0)
   136       {
   137            perror("tcsetattr");
   138            return -1;
   139       }
   140       return 0;
   141  }
