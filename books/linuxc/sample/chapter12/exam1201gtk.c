1  #include <gtk/gtk.h>  
2  int main(int argc,char *argv[])  
3  {  
4        GtkWidget *window;                  //一个GtkWidget类型的结构体指针
5        char title[]="test";  
6        gtk_init(&argc, &argv);                  //初始化GTK  
7        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);  //新建一个window窗体
8        gtk_window_set_title(GTK_WINDOW(window), title);   
9        //设置窗体的标题为title指向的字符串
10        gtk_widget_set_usize(GTK_WINDOW (window),300,150);  //设置窗体的大小
11        gtk_widget_set_uposition(GTK_WINDOW(window),200,200); //设置窗体的起始坐标位置
12        gtk_widget_show(window);   //显示窗体
13        gtk_main();    //进入主函数
14        return 0;  
15  }
