 1  ifndef _GLOBAL_H
     2  #define _GLOBAL_H
     3  #include <gnome.h>
     4  #include <gtk/gtk.h>
     5  #define XMAX    10                  /*��Ϸ����X�������ֵ*/
     6  #define YMAX    20                   /*��Ϸ����Y�������ֵ*/
     7  #define BLOCKWIDTH 20       /*������ÿһС����Ŀ��*/
     8  #define BLOCKHEIGHT 20       /*������ÿһС����ĸ߶�*/
     9  /*��Ϸ�����Ⱥ͸߶�*/
    10  #define GAMEAREAWIDTH ( XMAX*BLOCKWIDTH)
    11  #define GAMEAREAHEIGHT ( YMAX*BLOCKHEIGHT)
    12  /*��һ������ʾ����Ⱥ͸߶�*/
    13  #define NEXTAREAWIDTH   140
    14  #define NEXTAREAHEIGHT  120
    15  #define NUMBRICK   7                 /*�����ܵ�������*/
    16  /*�ֱ�����1��2��3��4�����÷���*/
    17  #define ONEROWSCORE        1
    18  #define TWOROWSCORE       3
    19  #define THREEROWSCORE    7
    20  #define FOURROWSCORE     13
    21  typedef struct _Position {
    22      gint x;
    23      gint y;
    24  } Position;
    25  /*������4��С������������Լ��������ھ���������ʼλ��*/
    26  typedef struct _block {
    27      Position blockpos[4];
    28      Position startpos;
    29      Position endpos;
    30  } Block;
    31  /*ÿ�ַ����4����̬*/
    32  typedef struct _brick{
    33      Block brick[4];
    34      gint index;
    35  }Brick;
    36  /*��������������Լ���ͼ������һ������ʾ��ͼ������*/
    37  typedef struct _KeyArg {
    38      GtkWidget *window;
    39      GtkWidget *game_area;
    40      GtkWidget *nextbrick_area;
    41  } KeyArg;
    42  #endif
