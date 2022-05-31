/*函数fscanf()示例*/   
#include<stdio.h> 
FILE* stream;  
int main(int argc,char *argv[])  
{  
    long l;  
    float fp;  
    char s[81];  
    char c;  
    if(argc != 2)
    {
      printf("文件参数错误！\n");
      return 0;
    }
    stream = fopen(*(argv+1),"w+");  
    if(stream == NULL)  
    {  
        printf("打开文件失败!\n");  
    }  
    else  
    {  
        fprintf(stream,"%s %d %f %c","a_string",6500,3.1415,  'x');  
        fseek(stream,0L,SEEK_SET);      /*定位文件*/  
        fscanf(stream,"%s",s);          /*格式化*/  
        fscanf(stream,"%ld",&l);  
        fscanf(stream,"%f",&fp);  
        fscanf(stream," %c",&c);  
        printf("%s\n",s);  
        printf("%ld\n",l);  
        printf("%f\n",fp);  
        printf("%c\n",c);  
        fclose(stream);/*关闭*/  
    }  
    return 0;
} 
