#include <gtk/gtk.h>

GtkWidget *spin;

//处理按键事件的回调函数
void button_deal(GtkWidget *widget,gpointer *data)
{
    gint year;
    year = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin)); //获得微调按纽的当前值
    g_print("Year:%d",year);   //在终端输出当前值
}

int main(int argc,char *argv[])
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *button;
    GtkObject *adjustment;
    gtk_init(&argc,&argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    box = gtk_hbox_new(FALSE,10); //创建组装盒
    gtk_container_add(GTK_CONTAINER(window),box); //将组装盒加入窗体
    adjustment = gtk_adjustment_new(2014,1900,2100,1,1,0); //创建微调对象
    spin = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment),0.5,0); //创建微调按纽
    gtk_box_pack_start(GTK_BOX(box),spin,TRUE,TRUE,5); //将微调按纽加入组装盒
    gtk_widget_show(spin);
    label = gtk_label_new("年");
    gtk_box_pack_start(GTK_BOX(box),label,TRUE,TRUE,0);
    gtk_widget_show(label);
    button = gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(box),button,TRUE,TRUE,0);
    gtk_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(button_deal),NULL);
    gtk_widget_show(button);
    gtk_widget_show(box);
    gtk_widget_show(window);
    gtk_main();
}
