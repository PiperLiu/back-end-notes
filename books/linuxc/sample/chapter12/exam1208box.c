#include <gtk/gtk.h>

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *button;      //窗口和按键的对应指针
  GtkWidget *box;         //组装盒
  char title[]="test"; 
  gtk_init(&argc,&argv);  //初始化函数

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //创建窗体
  gtk_window_set_title(GTK_WINDOW(window), title);   //设置窗体的标题为title指向的字符串
  gtk_widget_set_usize(GTK_WINDOW (window),300,150);  //设置窗体的大小
  gtk_widget_set_uposition(GTK_WINDOW(window),200,200); //设置窗体的起始坐标位置
  gtk_signal_connect(GTK_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL); 
  //登记窗体delete_event信号的回调函数
  box = gtk_hbox_new(FALSE,0);  //创建一个新的组合盒,横向

  button = gtk_button_new_with_label("Button1");  //创建一个新的按钮
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0);   //将按钮添加到组合盒中
  gtk_widget_show(button);
  
  button = gtk_button_new_with_label("Button2");  //创建一个新的按钮
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0);   //将按钮添加到组合盒中
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Button3");  //创建一个新的按钮
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0);   //将按钮添加到组合盒中
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Button4");  //创建一个新的按钮
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0);   //将按钮添加到组合盒中
  gtk_widget_show(button);

  gtk_container_add(GTK_CONTAINER(window),box); //把组合盒加入窗体
  gtk_widget_show(box);    //显示组合盒
  gtk_widget_show(window); //显示窗体
  gtk_main();
  return 0;
}

