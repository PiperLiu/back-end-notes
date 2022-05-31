#include <gtk/gtk.h>

//按键的回调处理函数
void button_deal(GtkWidget *widget,gpointer *data) 
{
  g_print("Button event:%s\n",data);//输出按键的状态
}

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *button;      //窗口和按键的对应指针
  char title[]="test"; 
  gtk_init(&argc,&argv);  //初始化函数
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //创建窗体
  gtk_window_set_title(GTK_WINDOW(window), title);   //设置窗体的标题为title指向的字符串
  gtk_widget_set_usize(GTK_WINDOW (window),300,150);  //设置窗体的大小
  gtk_widget_set_uposition(GTK_WINDOW(window),200,200); //设置窗体的起始坐标位置
  gtk_signal_connect(GTK_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL); 
  //登记窗体delete_event信号的回调函数
  button = gtk_check_button_new_with_label("Check Button"); //创建带标号的触发按钮
  gtk_signal_connect(GTK_OBJECT(button),"pressed",GTK_SIGNAL_FUNC(button_deal),"pressed"); 
  //登记按钮pressed信号的回调函数
  gtk_signal_connect(GTK_OBJECT(button),"released",GTK_SIGNAL_FUNC(button_deal),"released"); 
  //登记按钮released信号的回调函数
  gtk_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(button_deal),"clicked"); 
  //登记按钮clicked信号的回调函数
  gtk_signal_connect(GTK_OBJECT(button),"enter",GTK_SIGNAL_FUNC(button_deal),"enter"); 
  //登记按钮enter信号的回调函数
  gtk_signal_connect(GTK_OBJECT(button),"leave",GTK_SIGNAL_FUNC(button_deal),"leave"); 
  //登记按钮leave信号的回调函数
  gtk_signal_connect(GTK_OBJECT(button),"toggle",GTK_SIGNAL_FUNC(button_deal),"toggle");
  //登记按钮toggle信号的回调函数 
  gtk_container_add(GTK_CONTAINER(window),button); //把按钮加入窗体
  gtk_widget_show(button); //显示按钮
  gtk_widget_show(window); //显示窗体
  gtk_main();
  return 0;
}

