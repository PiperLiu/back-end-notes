#include <gtk/gtk.h>

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *box;
  GtkWidget *hrule;
  char title[]="test";
 
  gtk_init(&argc,&argv);  //初始化函数
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //创建窗体
  gtk_window_set_title(GTK_WINDOW(window), title);   //设置窗体的标题为title指向的字符串
  gtk_container_set_border_width(GTK_CONTAINER(window),10);
  gtk_widget_set_usize(GTK_WINDOW (window),400,100);  //设置窗体的大小
  gtk_widget_set_uposition(GTK_WINDOW(window),200,200); //设置窗体的起始坐标位置
  gtk_signal_connect(GTK_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL); 
  //登记窗体delete_event信号的回调函数

  box = gtk_vbox_new(FALSE,0);  //添加一个组装盒
  hrule = gtk_hruler_new();     //创建标尺
  gtk_ruler_set_metric(GTK_RULER(hrule),GTK_PIXELS);   //设置标尺的单位
  gtk_ruler_set_range(GTK_RULER(hrule),0,10,0,10);  //设置标尺的跨度和指示器初始位置
  gtk_box_pack_start(GTK_BOX(box),hrule,FALSE,FALSE,0);
  gtk_container_add(GTK_CONTAINER(window),box);

  gtk_widget_show(box);
  gtk_widget_show(hrule);
  gtk_widget_show(window);
  gtk_main();
}
