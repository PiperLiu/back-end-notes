#include <gtk/gtk.h>

GtkWidget *calendar;     //日历构件

//按键处理的回调函数
void button_event(GtkWidget *widget,gpointer *data)
{
  guint year;
  guint month;
  guint day;
  gtk_calendar_get_date(GTK_CALENDAR(calendar),&year,&month,&day);  //取得选择的年月日
  g_print("Year:%d Month:%d Day:%d\n",year,month,day);   //在终端输出
}

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *box;
  GtkWidget *button;
  gtk_init(&argc,&argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
  box=gtk_vbox_new(FALSE,10);//建立组装盒
  gtk_container_add(GTK_CONTAINER(window),box);//将组装盒加入窗体
  calendar = gtk_calendar_new();               //建立日历构件
  gtk_box_pack_start(GTK_BOX(box),calendar,TRUE,TRUE,5);  //将日历构件加入组装盒
  gtk_widget_show(calendar);/*显示日历构件*/
  button = gtk_button_new_with_label("确定");
  gtk_box_pack_start(GTK_BOX(box),button,TRUE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(button_event),NULL);
  gtk_widget_show(button);
  gtk_widget_show(box);
  gtk_widget_show(window);
  gtk_main();
}
