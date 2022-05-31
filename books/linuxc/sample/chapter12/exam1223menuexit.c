#include <gtk/gtk.h>

GtkWidget *CreateMenuItem(GtkWidget *MenuBar,char *test);
GtkWidget *CreateMenu(GtkWidget *MenuItem);

int main(int argc,char *argv[ ])
{
  GtkWidget *window;   //定义窗体
  GtkWidget *MenuBar;  //定义菜单条
  GtkWidget *box;      //定义组合框
  GtkWidget *MenuItemFile;  //定义文件子菜单
  gtk_init(&argc,&argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_usize(window,400,200);   //设置窗体大小
  g_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
  box = gtk_vbox_new(FALSE,0);      //创建纵向组合框
  gtk_container_add(GTK_CONTAINER(window),box);  //把组合框加入窗体
  MenuBar=gtk_menu_bar_new();  //创建菜单条
  gtk_box_pack_start(GTK_BOX(box),MenuBar,FALSE,TRUE,0);  //把菜单条加入组合框
  MenuItemFile = CreateMenuItem(MenuBar,"Fiel");    //调用创建菜单项函数*
  CreateMenu(MenuItemFile);/*调用创建子菜单函数*/
  CreateMenuItem(MenuBar,"Edit"); 
  CreateMenuItem(MenuBar,"View");
  CreateMenuItem(MenuBar,"Insert");
  CreateMenuItem(MenuBar,"Tool");
  gtk_widget_show(box);
  gtk_widget_show(MenuBar);
  gtk_widget_show(window);
  gtk_main();
}

GtkWidget *CreateMenuItem(GtkWidget *MenuBar,char *test)
{
  GtkWidget *MenuItem;
  MenuItem = gtk_menu_item_new_with_label(test);/*创建菜单项*/
  gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar),MenuItem);
  /*把菜单项加入菜单条，注意我们用gtk_menu_shell_append是为了程序的方便*/
  gtk_widget_show(MenuItem);
  return MenuItem;
}

GtkWidget *CreateMenu(GtkWidget *MenuItem)
{
  GtkWidget *Menu;/*定义子菜单*/
  GtkWidget *Exit;/*定义exit子菜单项*/
  Menu = gtk_menu_new();/*创建子菜单*/
  CreateMenuItem(Menu,"New");/*调用创建菜单项函数*/
  CreateMenuItem(Menu,"Open");
  CreateMenuItem(Menu,"Save");
  Exit = CreateMenuItem(Menu,"Exit");
  g_signal_connect(GTK_OBJECT(Exit),"activate",G_CALLBACK(gtk_main_quit),NULL);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuItem),Menu);/*把父菜单项于子菜单联系起来*/
  gtk_widget_show(Menu);
}
