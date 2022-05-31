#include <gtk/gtk.h>

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *button;      //窗口和按键的对应指针
  GtkWidget *table;       //组装表
  char title[]="test"; 
  gtk_init(&argc,&argv);  //初始化函数

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //创建窗体
  gtk_window_set_title(GTK_WINDOW(window), title);   //设置窗体的标题为title指向的字符串
  gtk_widget_set_usize(GTK_WINDOW (window),300,150);  //设置窗体的大小
  gtk_widget_set_uposition(GTK_WINDOW(window),200,200); //设置窗体的起始坐标位置
  gtk_signal_connect(GTK_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL); 
  //登记窗体delete_event信号的回调函数

  table = gtk_table_new(4,4,FALSE);   //创建一个4行4列的组合表

  button = gtk_button_new_with_label("Button1");  //创建一个按钮
  gtk_table_attach(GTK_TABLE(table),button,0,1,0,1,GTK_FILL,GTK_FILL,0,0);  //将按钮添加到组合表
  gtk_widget_show(button);  //显示按钮

  button = gtk_button_new_with_label("Button2");  //创建一个按钮
  gtk_table_attach(GTK_TABLE(table),button,1,2,1,2,GTK_FILL,GTK_FILL,0,0);  //将按钮添加到组合表
  gtk_widget_show(button);  //显示按钮

  button = gtk_button_new_with_label("Button3");  //创建一个按钮
  gtk_table_attach(GTK_TABLE(table),button,2,3,2,3,GTK_FILL,GTK_FILL,0,0);  //将按钮添加到组合表
  gtk_widget_show(button);  //显示按钮

  gtk_container_add(GTK_CONTAINER(window),table); //把组合盒加入窗体
  gtk_widget_show(table);    //显示组合盒
  gtk_widget_show(window); //显示窗体
  gtk_main();
  return 0;
}
