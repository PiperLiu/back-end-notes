#include <gtk/gtk.h>

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *button_box;
  GtkWidget *button;
  gtk_init(&argc,&argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request(window,300,50);
  gtk_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
  button_box = gtk_hbutton_box_new();     //创建按钮盒构件
  gtk_hbutton_box_set_spacing_default(5); //设置按钮间距
  gtk_hbutton_box_set_layout_default(GTK_BUTTONBOX_SPREAD);  //设置按钮盒布局
  gtk_container_add(GTK_CONTAINER(window),button_box);       //将按钮盒构件加入窗体
  gtk_widget_show(button_box);
  button=gtk_button_new_with_label("打开");
  gtk_container_add(GTK_CONTAINER(button_box),button);       //将按钮加入按钮盒构件
  gtk_widget_show(button);
  button=gtk_button_new_with_label("关闭");
  gtk_container_add(GTK_CONTAINER(button_box),button);
  gtk_widget_show(button);
  button=gtk_button_new_with_label("帮助");
  gtk_container_add(GTK_CONTAINER(button_box),button);
  gtk_widget_show(button);
  gtk_widget_show(window);
  gtk_main();
}
