#include <gtk/gtk.h>

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *frame;
  GtkWidget *button;
  GtkWidget *box;
  GSList *group = NULL;  //定义组
  gtk_init(&argc,&argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request(window,300,100);
  gtk_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
  frame = gtk_frame_new("性别");   //创建框架构件
  gtk_container_add(GTK_CONTAINER(window),frame);  //将框架构件加入窗体
  gtk_widget_show(frame);
  box = gtk_vbox_new(FALSE,0);  //创建组装框
  gtk_container_add(GTK_CONTAINER(frame),box);  //将组装框加入框架构件
  gtk_widget_show(box);
  button = gtk_radio_button_new_with_label(group,"男");  //创建按钮
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(button)); //将按钮加入组装框
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);
  gtk_widget_show(button);
  button = gtk_radio_button_new_with_label(group,"女");
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(button));
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);
  gtk_widget_show(button);
  gtk_widget_show(window);
  gtk_main();
}
