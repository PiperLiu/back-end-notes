1  #include <gtk/gtk.h>  
2  int main(int argc,char *argv[])  
3  {  
4        GtkWidget *window;                  //һ��GtkWidget���͵Ľṹ��ָ��
5        char title[]="test";  
6        gtk_init(&argc, &argv);                  //��ʼ��GTK  
7        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);  //�½�һ��window����
8        gtk_window_set_title(GTK_WINDOW(window), title);   
9        //���ô���ı���Ϊtitleָ����ַ���
10        gtk_widget_set_usize(GTK_WINDOW (window),300,150);  //���ô���Ĵ�С
11        gtk_widget_set_uposition(GTK_WINDOW(window),200,200); //���ô������ʼ����λ��
12        gtk_widget_show(window);   //��ʾ����
13        gtk_main();    //����������
14        return 0;  
15  }
