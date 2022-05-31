#include <gtk/gtk.h>

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *button;      //窗口和按键的对应指针
  GtkWidget *arrow;       //箭头对应的指针
  GtkWidget *box;         //组合盒对应的指针
 
  char title[]="test"; 
  gtk_init(&argc,&argv);  //初始化函数
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //创建窗体
  gtk_window_set_title(GTK_WINDOW(window), title);   //设置窗体的标题为title指向的字符串
  gtk_widget_set_usize(GTK_WINDOW (window),100,50);  //设置窗体的大小
  gtk_widget_set_uposition(GTK_WINDOW(window),200,200); //设置窗体的起始坐标位置
  gtk_signal_connect(GTK_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL); 
  //登记窗体delete_event信号的回调函数

  box = gtk_hbox_new(FALSE,0);   //创建一个组合盒
  gtk_container_add(GTK_CONTAINER(window),box); //将组合盒加入窗体

  button = gtk_button_new(); //创建不带标签的按钮
  arrow = gtk_arrow_new(GTK_ARROW_LEFT,GTK_SHADOW_OUT);   //创建指针
  gtk_container_add(GTK_CONTAINER(button),arrow);   //将指针加入按钮
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,TRUE,0);  //将按钮加入组装盒
  gtk_widget_show(arrow);
  gtk_widget_show(button);   //显示箭头和按钮

  button = gtk_button_new(); //创建不带标签的按钮
  arrow = gtk_arrow_new(GTK_ARROW_RIGHT,GTK_SHADOW_OUT);   //创建指针
  gtk_container_add(GTK_CONTAINER(button),arrow);   //将指针加入按钮
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,TRUE,0);  //将按钮加入组装盒
  gtk_widget_show(arrow);
  gtk_widget_show(button);   //显示箭头和按钮

  gtk_widget_show(box); //显示按钮
  gtk_widget_show(window); //显示窗体
  gtk_main();
  return 0;
}

