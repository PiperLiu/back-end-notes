#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#define SHM_SIZE 1024    //共享内存的大小
int main(int argc,char *argv[])
{
  int ret,        //临时变量
  pid,        //进程id
  sme_id,        //保存信号量描述符
  shm_id;        //保存共享内存描述符
  key_t     sme_key,    //保存信号量键值
  shm_key;    //保存共享内存键值
  char *shmp;        //指向共享内存的首地址
  struct shmid_ds dsbuf;    //定义共享内存信息结构变量
  struct sembuf lock = {0, -1, SEM_UNDO};        //信号量上锁操作的数组指针
  struct sembuf unlock = {0, 1, SEM_UNDO | IPC_NOWAIT};//信号量解锁操作的数组指针
  shm_key = ftok(*(argv+1), 2);    //获取信号量键值
  if(shm_key < 0)
  {
    perror("ftok");    //调用ftok函数出错
    exit(0);
  }
  sme_id = semget(shm_key, 1, IPC_CREAT | 0666);    //获取信号量ID
  if(sme_id < 0)
  {
    perror("semget");  //调用semget函数出错
    exit(0);
  }
  shm_key = ftok(*(argv+2), 1);    //获取共享内存键值
  if(shm_key < 0)
  {
    perror("ftok");
    exit(0);
  }
  shm_id = shmget(shm_key, SHM_SIZE, IPC_CREAT | 0666);        //获取共享内存ID
  if(shm_id < 0)
  {
    perror("shmget");
    exit(0);
  }
  shmp = shmat(shm_id, NULL, 0);        //映像共享内存
  if((int)shmp == -1)
  {
    perror("shmat");
    exit(0);
  }
  pid = fork();            //创建子进程
  if(pid < 0)
  {
    perror("fork");
    exit(0);
  }
  else if(pid == 0)        //子进程
  {
    ret = semctl(sme_id, 0, SETVAL, 1);        //初始化信号量，初值设为1
    if(ret == -1)
    {
      perror("semctl");
      exit(0);
    }
    ret = semop(sme_id, &lock, 1);        //申请访问共享资源，锁定临界资源
    if(ret == -1)
    {
      perror("semop lock");
      exit(0);
    }
    sleep(4);            //让子进程睡眠4秒
    strcpy(shmp, "hello\n");    //往共享内存写入数据
    if(shmdt((void *)shmp) < 0)    //使共享内存脱离进程地址空间
    {
      perror("shmdt");            
    }
    ret = semop(sme_id, &unlock, 1);        //解锁临界资源
    if(ret == -1)
    {
      perror("semop unlock");
      exit(0);
    }
  }
  else                //父进程
  {
    sleep(1);        //先让子进程运行
    ret = semop(sme_id, &lock, 1);        //申请访问共享资源，锁定临界资源
    if(ret == -1)
    {
      perror("semop lock");
      exit(0);
    }
    if(shmctl(shm_id, IPC_STAT, &dsbuf) < 0)    //获取共享内存信息
    {
      perror("shmctl");
      exit(0);
    }
    else            /* 共享内存的状态信息获取成功 */
    {
      printf("Shared Memory Information:\n");
      printf("\tCreator PID: %d\n", dsbuf.shm_cpid);         /* 输出创建共享内存进程的标识符 */
      printf("\tSize(bytes): %d\n",dsbuf.shm_segsz);         /* 输出共享内存的大小 */
      printf("\tLast Operator PID: %d\n",dsbuf.shm_lpid);     /* 输出上一次操作共享内存进程的标识符 */
      printf("Received message : %s\n", (char *)shmp);        /* 从共享内存中读取数据 */ 
    }
    if(shmdt((void *)shmp) < 0)    //使共享内存脱离进程地址空间
    {
      perror("shmdt");
      exit(0);            
    }        
    ret = semop(sme_id, &unlock, 1);        //解锁临界资源
    if(ret == -1)
    {
      perror("semop unlock");
      exit(0);
    }
    if(shmctl(shm_id, IPC_RMID, NULL) < 0)    /* 删除前面创建的共享内存 */ 
    {
      perror("shmctl");
      exit(0);            
    }
    ret = semctl(sme_id, 0, IPC_RMID, NULL);    //删除信号量
    if(ret == -1)
    {        
      perror("semctl");
      exit(0);        
    }
  }    
  return 0;
}

