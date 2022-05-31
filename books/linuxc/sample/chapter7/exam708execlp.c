/*调用execlp执行一个命令，其会从PATH环境变量所指定的目录中查找符合参数file的文件名
找到之后即开始执行该温江，然后以第二个之后的参数作为该文件的参数argv[0]、arg[1]
最后一个参数也必须用空指针NULL*/
#include<unistd.h>
int main(void)
{ 
    execlp("ls","ls","-al","/etc/passwd",(char * )0);  
    //执行bin下的ls命令，查找etc下的passwd文件，参数为ls
    return 0;
}
