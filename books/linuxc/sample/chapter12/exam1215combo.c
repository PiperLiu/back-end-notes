#include <gtk/gtk.h>

int main(int argc,char *argv[])
{
  GtkWidget *window;
  GtkWidget *combo;
  GList *glist = NULL;
  gtk_init(&argc,&argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
  combo = gtk_combo_new( );      //创建组合框
  glist = g_list_append(glist,"请选择"); //添加字符串到链表
  glist = g_list_append(glist,"数学");
  glist = g_list_append(glist,"语文");
  glist = g_list_append(glist,"外语");
  gtk_combo_set_popdown_strings(GTK_COMBO(combo),glist);
  gtk_container_add(GTK_CONTAINER(window),combo);
  gtk_widget_show(combo);
  gtk_widget_show(window);
  gtk_main();
}
