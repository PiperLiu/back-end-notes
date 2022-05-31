#include <gtk/gtk.h>

//按键的回调处理函数
void destroy(GtkWidget *widget,gpointer *data) 
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

//按键的回调函数
gint button_deal(GtkWidget *widget,gpointer gdata)
{
  GtkWidget *button;
  GtkWidget *dialog;
  
  dialog = gtk_dialog_new();   //新建对话框
  button = gtk_button_new_with_label("YES");
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),button,TRUE,TRUE,0);  //button到对话框的操作区
  gtk_signal_connect(GTK_OBJECT(button),"clicked",G_CALLBACK(gtk_main_quit),NULL);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("NO");
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),button,TRUE,TRUE,0);  //button到对话框的操作区
  gtk_signal_connect(GTK_OBJECT(button),"clicked",G_CALLBACK(destroy),dialog);
  gtk_widget_show(button);
  gtk_widget_show(dialog);
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
  gtk_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL); 
  //登记窗体delete_event信号的回调函数

  button = gtk_button_new_with_label("Exit");
  gtk_container_add(GTK_CONTAINER(window),button);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",G_CALLBACK(button_deal),NULL);
  gtk_widget_show(button);
  gtk_widget_show(window);
  gtk_main();
}
