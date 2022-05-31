 1  #i nclude "display.h"
     2  #include "control.h"
     3  #include "menu.h"
     4
     5  GtkWidget *game_area, *nextbrick_area;     /*游戏绘图区以及提示绘图区*/
     6  GtkWidget *nextbrick_label, *record_label,*level_label,*score_label,*line_label;
     7  KeyArg arg;
     8
     9  extern GdkColormap *colormap;
    10
    11  int main( int argc, char *argv[])
    12  {
    13      GtkWidget *window;
    14     GtkWidget *vbox1,*vbox2,*vbox3,*vbox4,*hbox;
    15
    16      GtkWidget *menubar;
    17      GtkWidget *gamemenu,  *helpmenu;
    18      GtkWidget *game,*newgame, *sep, *quit;
    19      GtkWidget *help,*content, *about;
    20      GtkAccelGroup *accel_group = NULL;
    21
    22     gtk_init(&argc, &argv);
    23
    24    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    25    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    26    gtk_window_set_default_size(GTK_WINDOW(window), 500, 360);
    27    gtk_window_set_title(GTK_WINDOW(window), "俄罗斯方块");
    28
    29    vbox1=gtk_vbox_new(FALSE,0);
    30   vbox2=gtk_vbox_new(TRUE,0);
    31   vbox3=gtk_vbox_new(FALSE,0);
    32   vbox4=gtk_vbox_new(TRUE,0);
    33   hbox=gtk_hbox_new(TRUE,20);
    34
    35    menubar = gtk_menu_bar_new();
    36      /*建立“游戏”,"帮助"菜单项容器  */
    37    gamemenu = gtk_menu_new();
    38    helpmenu= gtk_menu_new();
    39
    40     /*建立加速键容器*/
    41    accel_group = gtk_accel_group_new();
    42    gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
    43
    44  /*“游戏”菜单各菜单项*/
    45    game = gtk_menu_item_new_with_mnemonic("游戏(_G)");
    46    newgame = gtk_menu_item_new_with_mnemonic("新建游戏(_N)");
    47    sep = gtk_separator_menu_item_new();
    48    quit = gtk_menu_item_new_with_mnemonic("退出(_Q)");
    49
    50  /*创建“游戏”菜单各菜单项加速键*/
    51     gtk_widget_add_accelerator(newgame, "activate", accel_group,
    52        GDK_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    53    gtk_widget_add_accelerator(quit, "activate", accel_group,
    54        GDK_Q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    55  /*"游戏"菜单各菜单项与“游戏”菜单关联*/
    56    gtk_menu_item_set_submenu(GTK_MENU_ITEM(game), gamemenu);
    57    gtk_menu_append(GTK_MENU_SHELL(gamemenu), newgame);
    58    gtk_menu_append(GTK_MENU_SHELL(gamemenu), sep);
    59    gtk_menu_append(GTK_MENU_SHELL(gamemenu), quit);
    60
    61      /*连接“游戏”菜单项处理函数*/
    62      g_signal_connect (G_OBJECT(newgame),"activate",
    63                                      G_CALLBACK(NewGame), &arg);
    64      g_signal_connect(G_OBJECT(quit), "activate",
    65                                      G_CALLBACK(gtk_main_quit), NULL);
    66
    67      /*创建“帮助”菜单各菜单项*/
    68      help= gtk_menu_item_new_with_mnemonic("帮助(_H)");
    69      content= gtk_menu_item_new_with_mnemonic("内容(_C)");
    70      sep = gtk_separator_menu_item_new();
    71      about= gtk_menu_item_new_with_mnemonic("关于(_A)");
    72
    73  /*"帮助"菜单各菜单项与“帮助”菜单关联*/
    74    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), helpmenu);
    75    gtk_menu_append(GTK_MENU_SHELL(helpmenu), content);
    76    gtk_menu_append(GTK_MENU_SHELL(helpmenu),sep);
    77    gtk_menu_append(GTK_MENU_SHELL(helpmenu), about);
    78
    79      g_signal_connect (G_OBJECT(content),"activate",
    80                                      G_CALLBACK(HelpContent), NULL);
    81     g_signal_connect (G_OBJECT(about),"activate",
    82                                      G_CALLBACK(About), NULL);
    83   /*把菜单放到菜单栏上*/
    84    gtk_menu_bar_append(GTK_MENU_SHELL(menubar), game);
    85    gtk_menu_bar_append(GTK_MENU_SHELL(menubar), help);
    86      /*将菜单栏组装到vbox1*/
    87   gtk_box_pack_start(GTK_BOX(vbox1), menubar, FALSE, FALSE, 0);
    88
    89    /*创建游戏绘图区*/
    90    game_area=gtk_drawing_area_new();
    91    gtk_drawing_area_size ((GtkDrawingArea *)game_area,GAMEAREAWIDTH,GAMEAREAHEIGHT);
    92    /*将绘图区组装到水平盒子*/
    93    gtk_box_pack_start(GTK_BOX(hbox), game_area, FALSE, FALSE, 10);
    94
    95    /*下一方块提示区*/
    96    nextbrick_label=gtk_label_new("下一方块");
    97   gtk_box_pack_start(GTK_BOX(vbox3), nextbrick_label, FALSE, FALSE, 10);
    98
    99   /*创建显示下一方块绘图区*/
   100   nextbrick_area=gtk_drawing_area_new();
   101   gtk_drawing_area_size ((GtkDrawingArea *)nextbrick_area,NEXTAREAWIDTH,NEXTAREAHEIGHT);
   102
   103   gtk_box_pack_start(GTK_BOX(vbox3), nextbrick_area, FALSE, FALSE, 10);
   104   gtk_box_pack_start(GTK_BOX(vbox2), vbox3, FALSE, FALSE, 0);
   105
   106  /*成绩区*/
   107   record_label=gtk_label_new("成      绩");
   108   level_label=gtk_label_new("等 级：");
   109   line_label=gtk_label_new("行 数：");
   110   score_label=gtk_label_new("分 数：");
   111   gtk_box_pack_start(GTK_BOX(vbox4), record_label, FALSE, FALSE, 10);
   112   gtk_box_pack_start(GTK_BOX(vbox4), level_label, FALSE, FALSE, 10);
   113   gtk_box_pack_start(GTK_BOX(vbox4), line_label, FALSE, FALSE, 10);
   114   gtk_box_pack_start(GTK_BOX(vbox4), score_label, FALSE, FALSE, 10);
   115
   116   gtk_box_pack_start(GTK_BOX(vbox2), vbox4, FALSE, FALSE, 10);
   117   gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 10);
   118   gtk_box_pack_start(GTK_BOX(vbox1), hbox, FALSE, FALSE, 20);
   119   gtk_container_add(GTK_CONTAINER(window), vbox1);
   120
   121   colormap=gtk_widget_get_colormap(game_area);
   122
   123   gtk_widget_set_events (game_area, GDK_STRUCTURE_MASK|
   124                          GDK_EXPOSURE_MASK
   125                          |GDK_KEY_PRESS_MASK);
   126
   127   g_signal_connect (G_OBJECT(game_area),"configure_event",
   128                                          G_CALLBACK(gamearea_configure), NULL);
   129   g_signal_connect (G_OBJECT (game_area), "expose_event",
   130                                           G_CALLBACK( gamearea_expose), NULL);
   131    g_signal_connect (G_OBJECT(nextbrick_area),"configure_event",
   132                                          G_CALLBACK(nextbrickarea_configure), NULL);
   133   g_signal_connect (G_OBJECT (nextbrick_area), "expose_event",
   134                                            G_CALLBACK( nextbirckarea_expose), NULL);
   135       arg.window=window;
   136       arg.game_area=game_area;
   137       arg.nextbrick_area=nextbrick_area;
   138      g_signal_connect(G_OBJECT(window),"key-press-event",
   139                                          G_CALLBACK(KeyPress),(void*)&arg);
   140
   141     g_signal_connect_swapped(G_OBJECT(window), "delete_event",
   142          G_CALLBACK(gtk_main_quit), G_OBJECT(window));
   143
   144    gtk_widget_show_all(window);
   145    gtk_main();
   146    return 0;
   147  }
