#include <gtk/gtk.h>

GtkWidget *label;    //标签，由于其需要在回调函数中使用，必须使用全局变量

//处理按键的回调函数
void button_deal(GtkWidget* widget,gpointer* data)
{
  gtk_label_set(GTK_LABEL(label),(char *)data);   //修改标签的内容
}

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *button;      //窗口和按键的对应指针
  GtkWidget *box;         //组装盒
  GSList *group = NULL;   //单选框分组

  char title[]="test"; 
  gtk_init(&argc,&argv);  //初始化函数

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //创建窗体
  gtk_window_set_title(GTK_WINDOW(window), title);   //设置窗体的标题为title指向的字符串
  gtk_widget_set_usize(GTK_WINDOW (window),300,150);  //设置窗体的大小
  gtk_widget_set_uposition(GTK_WINDOW(window),200,200); //设置窗体的起始坐标位置
  gtk_signal_connect(GTK_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL); 
  //登记窗体delete_event信号的回调函数

  box = gtk_vbox_new(FALSE,0);  //创建一个新的组合盒,纵向

  label = gtk_label_new("PLZ check!");  //创建变迁
  gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,15); //将标签添加到组合盒
  
  button = gtk_radio_button_new_with_label(group,"Radio Button1");  //创建按钮
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(button));   //将按钮添加到组中
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0); //将按钮加入组合盒
  gtk_signal_connect(GTK_OBJECT(button),"pressed",GTK_SIGNAL_FUNC(button_deal),"Check Button1");
  //添加按键1的press事件处理回调函数
  gtk_widget_show(button);  //显示按键

  button = gtk_radio_button_new_with_label(group,"Radio Button2");  //创建按钮
  group = gtk_radio_button_group(GTK_RADIO_BUTTON(button));   //将按钮添加到组中
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0); //将按钮加入组合盒
  gtk_signal_connect(GTK_OBJECT(button),"pressed",GTK_SIGNAL_FUNC(button_deal),"Check Button2");
  //添加按键1的press事件处理回调函数
  gtk_widget_show(button);  //显示按键

  gtk_container_add(GTK_CONTAINER(window),box); //把组合盒加入窗体
  gtk_widget_show(label);  //显示标签
  gtk_widget_show(box);    //显示组合盒
  gtk_widget_show(window); //显示窗体
  gtk_main();
  return 0;
}
