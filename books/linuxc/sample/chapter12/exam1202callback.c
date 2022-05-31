#include <gtk/gtk.h>  

int main(int argc,char *argv[])  
{  
        GtkWidget *window;                  //一个GtkWidget类型的结构体指针
        char title[]="test";  
        gtk_init(&argc, &argv);                  //初始化GTK  
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);  //新建一个window窗体
        gtk_window_set_title(GTK_WINDOW(window), title);   //设置窗体的标题为title指向的字符串
        gtk_widget_set_usize(GTK_WINDOW (window),300,150);  //设置窗体的大小
        gtk_widget_set_uposition(GTK_WINDOW(window),200,200); //设置窗体的起始坐标位置
        g_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);  //退出的回调函数
        gtk_widget_show(window);   //显示窗体
        gtk_main();    //进入主函数
        return 0;  
}

