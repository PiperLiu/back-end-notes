#include  <gtk/gtk.h>

void ButtonEvent(GtkWidget *widget,gpointer *data);

int main(int argc,char *argv[ ])
{
  GtkWidget *window;
  GtkWidget *box;	//定义组装盒
  GtkWidget *toolbar;	//定义工具条
  GtkWidget *image;	//定义图片构件
  gtk_init(&argc,&argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_usize(window,1000,400);
  gtk_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
  box = gtk_vbox_new(FALSE,0); //创建组装盒
  toolbar = gtk_toolbar_new(); //创建工具条
  gtk_box_pack_start(GTK_BOX(box),toolbar,FALSE,TRUE,5);/*把工具条加入组装盒*/
  image = gtk_image_new_from_file("1.ico");//创建图片构件
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"office", "office",NULL,image,(GtkSignalFunc)ButtonEvent, "office");
  //创建工具条里的按钮
  image=gtk_image_new_from_file("2.ico");
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"bitcomet", "bitcomet",NULL,image,(GtkSignalFunc)ButtonEvent, "bitcomet");
  image = gtk_image_new_from_file("3.ico");
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"blender", "blender",NULL,image,(GtkSignalFunc)ButtonEvent, "blender");
  image = gtk_image_new_from_file("4.ico");
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"coeur", "coeur",NULL,image,(GtkSignalFunc)ButtonEvent, "coeur");
  image = gtk_image_new_from_file("5.ico");
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"PS", "PS",NULL,image,(GtkSignalFunc)ButtonEvent, "PS");
  gtk_container_add(GTK_CONTAINER(window),box);
  gtk_widget_show(toolbar);  
  gtk_widget_show(box);
  gtk_widget_show(window); 
  gtk_main();
}

void ButtonEvent(GtkWidget *widget,gpointer *data) /*回调函数*/
{
  g_print("%s\n",data);
}
