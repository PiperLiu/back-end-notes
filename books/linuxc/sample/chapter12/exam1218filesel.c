#include <gtk/gtk.h>

GtkWidget *FileSelection;   //文件选择构件

//在终端中输出当前选择的文件名称
void OpenFile(GtkWidget *widget,gpointer *data)
{
  g_print("%s\n",gtk_file_selection_get_filename(GTK_FILE_SELECTION(FileSelection)));
}

//按键处理的回调函数
void button_event(GtkWidget *widget,gpointer *data)
{
  FileSelection=gtk_file_selection_new("选择文件");   //创建文件选择构件
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(FileSelection),"*.txt");
  gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(FileSelection)->ok_button),"clicked",GTK_SIGNAL_FUNC(OpenFile),NULL);
  //捕捉打开按纽的”clicked”信号
  gtk_widget_show(FileSelection);
}
 

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *button;
  gtk_init(&argc,&argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request(window,200,100);/*调整窗口大小*/
  gtk_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
  button=gtk_button_new_with_label("打开文件");/*常见按纽*/
  gtk_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(button_event),NULL);
  gtk_container_add(GTK_CONTAINER(window),button);
  gtk_widget_show(button);
  gtk_widget_show(window);
  gtk_main();
}
