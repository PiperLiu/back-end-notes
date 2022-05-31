     1  #ifndef     _CONTROL_H
     2  #define     _CONTROL_H
     3  #include "global.h"
     4  gboolean time_handler(GtkWidget *widget);
     5  gint PositionCorrect(gint newind,gint srcx, gint srcy);
     6  gint KeyPress(GtkWidget *widget, GdkEventKey *event, gpointer arg);
     7  void initBrick();
     8  #endif

///===========================

     1  #include "control.h"
     2
     3  extern guint tid;
     4  extern guint mytimer;
     5  extern GtkWidget *level_label,*score_label,*line_label;
     6
     7  gint g_Filled[XMAX][YMAX];  /*标记游戏区域是否有方块*/
     8  gboolean bStop=TRUE;        /*标识游戏是否结束*/
     9  gboolean bPause=FALSE;    /*标识游戏是否暂停*/
    10  guint nLevel,nLine,nScore;     /*游戏等级、消去的行数以及所得总分数*/
    11  Brick g_allbrick[ NUMBRICK];    /*所有方块及其默认形态*/
    12  Brick g_curbrick;                       /*当前方块及其形态*/
    13  Brick g_nextbrick;                      /*下一方块及其形态*/
    14
    15  gint g_curbrickind;             /*当前方块形态索引*/
    16  gint g_nextbrickind;           /*下一方块形态索引*/
    17  gint g_curbrickx=3;             /* 方块初始位置x坐标*/
    18  gint g_curbricky=0;             /* 方块初始位置y坐标*/
    19
    20  /*定时器超时处理函数*/
    21  gboolean time_handler(GtkWidget *widget)
    22  {
    23    GdkEvent t_event;
    24
    25    if (widget->window == NULL || TRUE==bStop)
    26        return FALSE;
    27      t_event.type=GDK_KEY_PRESS;
    28      ((GdkEventKey*)&t_event)->window=widget->window;
    29      ((GdkEventKey*)&t_event)->send_event=TRUE;
    30      ((GdkEventKey*)&t_event)->time=0;
    31      ((GdkEventKey*)&t_event)->state=0;
    32      ((GdkEventKey*)&t_event)->keyval=GDK_Down;
    33      ((GdkEventKey*)&t_event)->length=4;
    34      ((GdkEventKey*)&t_event)->string="Down";
    35      ((GdkEventKey*)&t_event)->hardware_keycode=0;
    36      ((GdkEventKey*)&t_event)->group=0;
    37      gdk_event_put(&t_event);
    38      return TRUE;
    39  }

///////==========

1  /*检测方块是否到达边界以及目标区域是否已有方块*/
     2  gint PositionCorrect(gint newind,gint srcx, gint srcy)
     3  {
     4
     5      gint i, sum=0,ind=g_curbrick.index;
     6      gint x1,y1,x, y;
     7
     8      /*把方块当前区域清除*/
     9      for(i=0;i<4;i++){
    10          x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
    11          y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
    12          g_Filled[x][y]=0;
    13      }
    14      /* 目标区域标记为当前方块，判断在目标区域是否已有方块以及是否到达边界*/
    15      for(i=0;i<4;i++){
    16          x1=g_curbrick.brick[newind].blockpos[i].x;
    17          y1=g_curbrick.brick[newind].blockpos[i].y;
    18          x1+=srcx;
    19          y1+=srcy;
    20          if(x1<0||x1>=XMAX||y1<0||y1>=YMAX)
    21              sum++;
    22          else
    23              sum+=g_Filled[x1][y1];
    24      }
    25
    26      /* 恢复方块到原来占用区域*/
    27      for(i=0;i<4;i++){
    28          x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
    29          y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
    30          g_Filled[x][y]=1;
    31      }
    32      /* 返回判断结果 */
    33      if(sum>=1)
    34          return 0;  //碰到边界
    35      else
    36          return 1;
    37  }
    
    ///---==============
    
     1 gint KeyPress(GtkWidget *widget, GdkEventKey *event, gpointer arg)
    2 {
    3     gint ind,oldind,x,y,ret, i;
    4     Position oldstart,oldend,newstart,newend;
    5     GdkEvent t_event;
    6     KeyArg *arg1=arg;
    7     oldstart.x=g_curbrickx;
    8     oldstart.y=g_curbricky;
    9     ind=g_curbrick.index;
    10     oldind=g_curbrick.index;
    11     oldend=g_curbrick.brick[ind].endpos;
    12     oldend.x+=g_curbrickx;
    13     oldend.y+=g_curbricky;
    14     ret=0;
    15
    16    if(bPause==TRUE && event->keyval!=GDK_Return)
    17     return  0;
    18     switch(event->keyval)
    19     {
    20         case GDK_Return:
    21               if(bPause==FALSE)
    22                 bPause=TRUE;
    23               else
    24                 bPause=FALSE;
    25                break;
    26
    27         case GDK_space:
    28         {
    29             ind=(g_curbrick.index+1)%4;
    30              ret=PositionCorrect(ind,g_curbrickx, g_curbricky);
    31              if(ret==1){
    32                  for(i=0;i<4;i++){
    33                      x=g_curbrick.brick[oldind].blockpos[i].x+g_curbrickx;
    34                      y=g_curbrick.brick[oldind].blockpos[i].y+g_curbricky;
    35                      g_Filled[x][y]=0;
    36                  }
    37                  g_curbrick.index=ind;   //设为新的index
    38                  for(i=0;i<4;i++){
    39                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
    40                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
    41                      g_Filled[x][y]=1;
    42                  }
    43              }
    44          }
    45           break;
    46
    47           case GDK_Down:
    48           {
    49              ret=PositionCorrect(ind,g_curbrickx, g_curbricky+1);
    50              if(ret==1){
    51                  for(i=0;i<4;i++){
    52                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
    53                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
    54                      g_Filled[x][y]=0;
    55                  }
    56                   g_curbricky++;
    57                 for(i=0;i<4;i++){
    58                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
    59                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
    60                      g_Filled[x][y]=1;
    61                  }
    62              }
    63              else{   /*到达底部*/
    64                  ret=1;
    65                  if(0==g_curbricky)  //到达最顶部，游戏结束
    66                  {
    67                      bStop=TRUE;
    68                      GtkWidget *dialog;
    69                      dialog = gtk_message_dialog_new((GtkWindow *)widget,
    70                                  GTK_DIALOG_DESTROY_WITH_PARENT,
    71                                  GTK_MESSAGE_ERROR,
    72                                  GTK_BUTTONS_OK,
    73                                  "游戏结束！");
    74                      gtk_window_set_title(GTK_WINDOW(dialog), "游戏结束");
    75                      gtk_dialog_run(GTK_DIALOG(dialog));
    76                      gtk_widget_destroy(dialog);
    77                      gamearea_configure(arg1->game_area,NULL);
    78                      nextbrickarea_configure(arg1->nextbrick_area,NULL);
    79                      return  1;
    80                  }
    81
    82                  /*消掉被方块填满的行，同时将上方区域下移一行*/
    83                  gint srcy=YMAX-1;
    84                  gint dsty=YMAX-1;
    85                  guint fullRow = 0;    //
    86                  for(y=YMAX-1;y>=0;y--){
    87                        if(srcy!=dsty){                           /*需要下移*/
    88                                  for( x=0;x<XMAX;x++){      /*复制需要移动的区域*/
    89                                      g_Filled[x][dsty]=g_Filled[x][srcy];
    90                                  }
    91                          }
    92                        srcy--;
    93                        /*统计被方块填充满的行*/
    94                        gint sumrow=0;
    95                        for( x=0;x<XMAX;x++){
    96                              sumrow+=g_Filled[x][dsty];
    97                        }
    98                        if(sumrow<XMAX)   /*该行未填充满*/
    99                              dsty--;
   100                        else
   101                              ++fullRow;
   102                     }
   103                  for(y=0;y<=dsty;y++){
   104                      for(x=0;x<XMAX;x++)
   105                          g_Filled[x][y]=0;
   106                  }
   107                  /*根据消去的行数判断所得分数*/
   108                  switch (fullRow)
   109                  {
   110                      case 0:
   111                          break;
   112                      case 1:
   113                          nScore += ONEROWSCORE;
   114                          break;
   115                      case 2:
   116                          nScore += TWOROWSCORE;
   117                      break;
   118                      case 3:
   119                          nScore += THREEROWSCORE;
   120                          break;
   121                      case 4:
   122                          nScore += FOURROWSCORE;
   123                          break;
   124                      default:
   125                          g_print("错误!\n");
   126                          return -1;
   127                  }
   128                   nLine+=fullRow;  //更新行数
   129                  if(fullRow!=0) {
   130                  if(nLine<=20)
   131                  {
   132                      nLevel=1;
   133                      mytimer=1000;
   134                    }
   135                  else if(nLine<=40)
   136                  {
   137                      nLevel=2;
   138                      mytimer=800;
   139                  }
   140                  else if(nLine <=60)
   141                  {
   142                      nLevel=3;
   143                      mytimer=600;
   144                  }
   145                   else if(nLine<=8)
   146                  {
   147                      nLevel=4;
   148                      mytimer=400;
   149                  }
   150                   else
   151                   {
   152                       nLevel=5;
   153                       mytimer=200;
   154                   }
   155                  if(tid)
   156                              g_source_remove  (tid);
   157             tid=g_timeout_add(mytimer, (GSourceFunc) time_handler, (GtkWidget *) arg1->window);
   158                    time_handler((GtkWidget *)arg1->window);
   159                  }
   160                  gchar buffer[20];
   161                  sprintf (buffer, "等 级：%d",nLevel);
   162                  gtk_label_set_text(GTK_LABEL(level_label),buffer);
   163                  sprintf (buffer, "行 数：%d",nLine);
   164                  gtk_label_set_text(GTK_LABEL(line_label),buffer);
   165                  sprintf (buffer, "分 数：%d",nScore);
   166                  gtk_label_set_text(GTK_LABEL(score_label),buffer);
   167
   168                  g_curbrickind=g_nextbrickind;
   169                  g_curbrick=g_allbrick[g_curbrickind];
   170                  g_nextbrickind=g_random_int_range(0, NUMBRICK);
   171                  g_nextbrick=g_allbrick[g_nextbrickind];
   172                  ind=g_curbrick.index;
   173                  g_curbrickx=3;
   174                  g_curbricky=0;
   175                  for(i=0;i<4;i++){
   176                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
   177                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
   178                      g_Filled[x][y]=1;
   179                  }
   180                  oldstart.x=0;oldstart.y=0;
   181                  oldend.x=XMAX-1;oldend.y=YMAX-1;
   182              }
   183           }
   184           break;
   185
   186          case GDK_Right:
   187          {
   188              ret=PositionCorrect(ind,g_curbrickx+1, g_curbricky);
   189              if(ret==1){
   190                  for(i=0;i<4;i++){
   191                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
   192                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
   193                      g_Filled[x][y]=0;
   194                  }
   195                  g_curbrickx++;
   196                  for(i=0;i<4;i++){
   197                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
   198                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
   199                      g_Filled[x][y]=1;
   200                  }
   201              }
   202          }
   203          break;
   204
   205          case GDK_Left:
   206          {
   207              ret=PositionCorrect(ind,g_curbrickx-1, g_curbricky);
   208              if(ret==1){
   209                  for(i=0;i<4;i++){
   210                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
   211                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
   212                      g_Filled[x][y]=0;
   213                  }
   214                  g_curbrickx--;
   215                  for(i=0;i<4;i++){
   216                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
   217                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
   218                      g_Filled[x][y]=1;
   219                  }
   220              }
   221          }
   222          break;
   223      }
   224      if(ret==1){
   225          newstart.x=g_curbrickx;
   226          newstart.y=g_curbricky;
   227          ind=g_curbrick.index;
   228          newend=g_curbrick.brick[ind].endpos;
   229          newend.x=newend.x+newstart.x;
   230          newend.y=newend.y+newstart.y;
   231          if(newstart.x>oldstart.x)    newstart.x=oldstart.x;
   232          if(newstart.y>oldstart.y)    newstart.y=oldstart.y;
   233          if(newend.x<oldend.x)   newend.x=oldend.x;
   234          if(newend.y<oldend.y)   newend.y=oldend.y;
   235
   236          /*重绘游戏区*/
   237          t_event.type=GDK_EXPOSE;
   238          ((GdkEventExpose*)&t_event)->window=((GtkWidget* )arg1->game_area)->window;
   239          ((GdkEventExpose*)&t_event)->send_event=TRUE;
   240          ((GdkEventExpose*)&t_event)->area.x=newstart.x*BLOCKWIDTH;
   241          ((GdkEventExpose*)&t_event)->area.y=newstart.y*BLOCKHEIGHT;
   242          ((GdkEventExpose*)&t_event)->area.width=(newend.x-newstart.x+1)*BLOCKWIDTH;
   243          ((GdkEventExpose*)&t_event)->area.height=(newend.y-newstart.y+1)*BLOCKHEIGHT;
   244          ((GdkEventExpose*)&t_event)->region=gdk_region_rectangle(&((GdkEventExpose*)&t_event)->area);
   245          ((GdkEventExpose*)&t_event)->count=0;
   246          gdk_event_put(&t_event);
   247
   248          /* 重绘下一方块区 */
   249          t_event.type=GDK_EXPOSE;
   250          ((GdkEventExpose*)&t_event)->window=((GtkWidget* )arg1->nextbrick_area)->window;
   251          ((GdkEventExpose*)&t_event)->send_event=TRUE;
   252          ((GdkEventExpose*)&t_event)->area.x=0;
   253          ((GdkEventExpose*)&t_event)->area.y=0;
   254          ((GdkEventExpose*)&t_event)->area.width=NEXTAREAWIDTH;
   255          ((GdkEventExpose*)&t_event)->area.height=NEXTAREAHEIGHT;
   256          ((GdkEventExpose*)&t_event)->region=gdk_region_rectangle(&((GdkEventExpose*)&t_event)->area);
   257          ((GdkEventExpose*)&t_event)->count=0;
   258          gdk_event_put(&t_event);
   259      }
   260      return 0;
   261  }


///=====================
  1  void initBrick()
     2  {
     3      /*每种方块4种形态下4个小方块坐标以及方块所在矩形区域的开始位置和结束位置*/
     4      gint    par[NUMBRICK][48]={{0,1,1,1,2,1,1,0,0,0,2,1,\
     5                                  0,0,0,1,0,2,1,1,0,0,1,2,\
     6                                   0,0,1,0,2,0,1,1,0,0,2,1,\
     7                                   1,0,1,1,1,2,0,1,0,0,1,2},\
     8
     9                                  {0,0,1,0,2,0,3,0,0,0,3,0,\
    10                                   0,0,0,1,0,2,0,3,0,0,0,3,\
    11                                   0,0,1,0,2,0,3,0,0,0,3,0,\
    12                                   0,0,0,1,0,2,0,3,0,0,0,3},\
    13
    14                                  {0,1,1,1,2,1,2,0,0,0,2,1,\
    15                                   0,0,0,1,0,2,1,2,0,0,1,2,\
    16                                   0,0,1,0,2,0,0,1,0,0,2,1,\
    17                                   0,0,1,0,1,1,1,2,0,0,1,2},\
    18
    19                                  {0,0,1,0,0,1,1,1,0,0,1,1,\
    20                                   0,0,0,1,1,0,1,1,0,0,1,1,\
    21                                   0,0,1,0,0,1,1,1,0,0,1,1,\
    22                                   0,0,0,1,1,0,1,1,0,0,1,1},\
    23
    24                                  {0,0,0,1,1,1,1,2,0,0,1,2,\
    25                                   1,0,2,0,0,1,1,1,0,0,2,1,\
    26                                   0,0,0,1,1,1,1,2,0,0,1,2,\
    27                                   1,0,2,0,0,1,1,1,0,0,2,1},
    28
    29                                   {0,0,0,1,1,1,2,1,0,0,2,1,\
    30                                    1,0,0,0,0,1,0,2,0,0,1,2,\
    31                                    0,0,1,0,2,0,2,1,0,0,2,1,\
    32                                    1,0,1,1,1,2,0,2,0,0,1,2},\
    33
    34                                   {1,0,1,1,0,1,0,2,0,0,1,2,\
    35                                    0,0,1,0,1,1,2,1,0,0,2,1,\
    36                                    1,0,1,1,0,1,0,2,0,0,1,2,\
    37                                    0,0,1,0,1,1,2,1,0,0,2,1}} ;
    38       int i,j,k,l;
    39       for(i=0;i<NUMBRICK;i++)
    40       {
    41           g_allbrick[i].index=0;
    42           for(j=0;j<4;j++)
    43           {
    44                for(k=0;k<4;k++)
    45              {
    46                  g_allbrick[i].brick[j].blockpos[k].x= par[i][j*12+2*k];
    47                  g_allbrick[i].brick[j].blockpos[k].y= par[i][j*12+2*k+1];
    48              }
    49              g_allbrick[i].brick[j].startpos.x=par[i][j*12+8];
    50              g_allbrick[i].brick[j].startpos.y=par[i][j*12+9];
    51              g_allbrick[i].brick[j].endpos.x=par[i][j*12+10];
    52              g_allbrick[i].brick[j].endpos.y=par[i][j*12+11];
    53           }
    54       }
    55      /*设定当前方块及下一方块*/
    56      g_curbrickind=g_random_int_range(0, NUMBRICK);
    57      g_curbrick=g_allbrick[g_curbrickind];
    58      g_nextbrickind=g_random_int_range(0, NUMBRICK);
    59      g_nextbrick=g_allbrick[g_nextbrickind];
    60   }

