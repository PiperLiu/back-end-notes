 1  ifndef _GLOBAL_H
     2  #define _GLOBAL_H
     3  #include <gnome.h>
     4  #include <gtk/gtk.h>
     5  #define XMAX    10                  /*游戏区域X坐标最大值*/
     6  #define YMAX    20                   /*游戏区域Y坐标最大值*/
     7  #define BLOCKWIDTH 20       /*方块中每一小方块的宽度*/
     8  #define BLOCKHEIGHT 20       /*方块中每一小方块的高度*/
     9  /*游戏区域宽度和高度*/
    10  #define GAMEAREAWIDTH ( XMAX*BLOCKWIDTH)
    11  #define GAMEAREAHEIGHT ( YMAX*BLOCKHEIGHT)
    12  /*下一方块提示区宽度和高度*/
    13  #define NEXTAREAWIDTH   140
    14  #define NEXTAREAHEIGHT  120
    15  #define NUMBRICK   7                 /*方块总的类型数*/
    16  /*分别消掉1、2、3、4行所得分数*/
    17  #define ONEROWSCORE        1
    18  #define TWOROWSCORE       3
    19  #define THREEROWSCORE    7
    20  #define FOURROWSCORE     13
    21  typedef struct _Position {
    22      gint x;
    23      gint y;
    24  } Position;
    25  /*方块中4个小方块各自坐标以及方块所在矩形区域起始位置*/
    26  typedef struct _block {
    27      Position blockpos[4];
    28      Position startpos;
    29      Position endpos;
    30  } Block;
    31  /*每种方块的4种形态*/
    32  typedef struct _brick{
    33      Block brick[4];
    34      gint index;
    35  }Brick;
    36  /*保存程序主窗口以及绘图区和下一方块提示绘图区参数*/
    37  typedef struct _KeyArg {
    38      GtkWidget *window;
    39      GtkWidget *game_area;
    40      GtkWidget *nextbrick_area;
    41  } KeyArg;
    42  #endif
