#include <gtk/gtk.h>

GtkWidget *name;  //用户名
GtkWidget *passwd;  //密码

//处理按键的回调函数
void button_deal(GtkWidget* widget,gpointer* data)
{
  const gchar *uname;
  const gchar *upasswd;

  uname = (gchar *)malloc(sizeof(gchar));    		//分配内存空间
  upasswd = (gchar *)malloc(sizeof(gchar));   		//分配内存空间
  uname = gtk_entry_get_text(GTK_ENTRY(name));  	//获得用户名
  upasswd = gtk_entry_get_text(GTK_ENTRY(passwd)); 	//获得密码
  g_print("Name:%s/n",uname);
  g_print("Passwd:%s\n",upasswd);
}

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *button;      //窗口和按键的对应指针
  GtkWidget *box;         //组装盒
  GtkWidget *label;

  char title[]="test"; 
  gtk_init(&argc,&argv);  //初始化函数

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //创建窗体
  gtk_window_set_title(GTK_WINDOW(window), title);   //设置窗体的标题为title指向的字符串
  gtk_widget_set_usize(GTK_WINDOW (window),300,150);  //设置窗体的大小
  gtk_widget_set_uposition(GTK_WINDOW(window),200,200); //设置窗体的起始坐标位置
  gtk_signal_connect(GTK_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL); 
  //登记窗体delete_event信号的回调函数

  box = gtk_vbox_new(FALSE,0);   //创建组合框
  label = gtk_label_new("name:"); //创建标签
  gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,5);  //将标签加入组合框
  gtk_widget_show(label);   //显示标签  

  name = gtk_entry_new();  //创建输入构件
  gtk_entry_set_visibility(GTK_ENTRY(name),TRUE);  //设置字符串可见
  gtk_box_pack_start(GTK_BOX(box),name,FALSE,FALSE,5);//将输入构件加入组合盒
  gtk_widget_show(name); //显示输入构件

  label = gtk_label_new("passwd:"); //创建标签
  gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,5);  //将标签加入组合框
  gtk_widget_show(label);   //显示标签  

  passwd = gtk_entry_new();  //创建输入构件
  gtk_entry_set_visibility(GTK_ENTRY(passwd),FALSE);  //设置字符串不可见
  gtk_box_pack_start(GTK_BOX(box),passwd,FALSE,FALSE,5);//将输入构件加入组合盒
  gtk_widget_show(passwd); //显示输入构件

  button = gtk_button_new_with_label("ENTER");  //创建按钮
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);  //将按钮加入组合盒
  gtk_signal_connect(GTK_OBJECT(button),"pressed",GTK_SIGNAL_FUNC(button_deal),"enter");  //声明回调函数
  gtk_widget_show(button);  //显示按钮

  gtk_container_add(GTK_CONTAINER(window),box); //把组合盒加入窗体
  gtk_widget_show(box);    //显示组合盒
  gtk_widget_show(window); //显示窗体
  gtk_main();
  return 0;
}




