#include <stdio.h>
int main(void)   //没有参数
{
  int a,b,sum;
  char str[30]; //字符串存放
  printf("please input a,b!\n");   
  scanf("%d%d",&a,&b);      //输入两个整数
  sum = a * b;               //计算乘积
  printf("the sum is %d\n",sum);  //输出计算结果
  printf("please input the string\n");
  scanf("%s",str);
  printf("the string is %s\n",str);   //打印刚刚输入的字符串
  return 0;
}

