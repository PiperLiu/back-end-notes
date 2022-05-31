#include<gtk/gtk.h>
int main(int argc,char *argv[])
{
   GtkWidget *window;
   GtkWidget *menu;
   GtkWidget *menubar;
   GtkWidget *rootmenu;
   GtkWidget *menuitem;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window),
   g_locale_to_utf8("下拉菜单",-1,NULL,NULL,NULL));
   gtk_widget_set_usize (window, 200, 40);
   /*下面是创建一个新菜单，然后创建3个菜单项，并把这3个菜单项加入到菜单中*/
   menu = gtk_menu_new();
   menuitem = gtk_menu_item_new_with_label
(g_locale_to_utf8("新建",-1,NULL,NULL,NULL));
   gtk_menu_append (GTK_MENU(menu),menuitem);
   menuitem = gtk_menu_item_new_with_label
(g_locale_to_utf8("打开",-1,NULL,NULL,NULL));
   gtk_menu_append (GTK_MENU(menu),menuitem);
   menuitem = gtk_menu_item_new_with_label
(g_locale_to_utf8("关闭",-1,NULL,NULL,NULL));
   gtk_menu_append (GTK_MENU(menu),menuitem);
   menuitem = gtk_menu_item_new_with_label 
(g_locale_to_utf8("保存",-1,NULL,NULL,NULL));
   gtk_menu_append (GTK_MENU(menu),menuitem);
   rootmenu = gtk_menu_item_new_with_label
(g_locale_to_utf8("文件 ",-1,NULL,NULL,NULL)); /*创建一个主菜单*/
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),menu);
                                   /*将菜单加入到主菜单中*/
   menubar = gtk_menu_bar_new();   /*创建菜单条*/
   gtk_menu_bar_append(GTK_MENU_BAR(menubar),rootmenu);
   /*将主菜单条加入到菜单条中*/
   /*下面使用同样的方法，创建第二组菜单*/
   menu = gtk_menu_new();
   menuitem = gtk_menu_item_new_with_label
(g_locale_to_utf8("复制",-1,NULL,NULL,NULL));
   gtk_menu_append (GTK_MENU(menu),menuitem);
   menuitem = gtk_menu_item_new_with_label
(g_locale_to_utf8("剪切",-1,NULL,NULL,NULL));
   gtk_menu_append(GTK_MENU(menu),menuitem);
   menuitem = gtk_menu_item_new_with_label
(g_locale_to_utf8("粘贴",-1,NULL,NULL,NULL));
   gtk_menu_append(GTK_MENU(menu),menuitem);
   menuitem = gtk_menu_item_new_with_label
(g_locale_to_utf8("删除",-1,NULL,NULL,NULL));
   gtk_menu_append(GTK_MENU(menu),menuitem);
   rootmenu = gtk_menu_item_new_with_label
(g_locale_to_utf8("编辑 ",-1,NULL,NULL,NULL));
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),menu);
   gtk_menu_bar_append(GTK_MENU_BAR(menubar),rootmenu);
   gtk_container_add(GTK_CONTAINER(window),menubar);  /*将菜单条加入到窗口中*/
   gtk_widget_show_all (window); /*显示窗口中的所有元件*/
   gtk_main();
   return 0;
}
