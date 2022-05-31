     1  #ifndef _DISPLAY_H
     2  #define _DISPLAY_H
     3  #include   "global.h"
     4  gint gamearea_configure (GtkWidget *widget, GdkEventConfigure *event);
     5  gint nextbrickarea_configure (GtkWidget *widget, GdkEventConfigure *event);
     6  gint  gamearea_expose (GtkWidget *widget, GdkEventExpose *event);
     7  gint  nextbirckarea_expose (GtkWidget *widget, GdkEventExpose *event);
     8  #endif
//==================

1  #include    "display.h"
     2
     3   extern gboolean bStop;
     4   extern gint g_Filled[XMAX][YMAX];
     5
     6  GdkPixmap *game_pixmap,*nextbrick_pixmap;   /*游戏绘图区以及提示区后端位图*/
     7  GdkColor color;
     8  GdkColormap *colormap;
     9  GdkGC *gc,*gc1;
    10
    11  /* 创建适当大小的游戏区及提示区后端位图 */
    12  gint gamearea_configure (GtkWidget *widget, GdkEventConfigure *event)
    13  {
    14          if (game_pixmap)
    15              gdk_pixmap_unref(game_pixmap);
    16          game_pixmap = gdk_pixmap_new(widget->window, GAMEAREAWIDTH,
    17                                             GAMEAREAHEIGHT, -1);
    18          gc =gdk_gc_new(game_pixmap);
    19           if (gdk_color_parse("orange", &color))
    20           {
    21                 if (gdk_colormap_alloc_color(colormap, &color, FALSE, TRUE))
    22                     gdk_gc_set_foreground (gc, &color );
    23          }
    24
    25           gc1 =gdk_gc_new(game_pixmap);
    26            if (gdk_color_parse("green", &color))
    27              {
    28                  if (gdk_colormap_alloc_color(colormap, &color, FALSE, TRUE))
    29                      gdk_gc_set_foreground (gc1, &color );
    30              }
    31           gdk_draw_rectangle (game_pixmap,  widget->style->white_gc, TRUE,  0, 0,
    32                               GAMEAREAWIDTH, GAMEAREAHEIGHT);
    33          gdk_draw_pixmap(widget->window, widget->style->white_gc, game_pixmap,0,0,0,0,
    34                                      GAMEAREAWIDTH, GAMEAREAHEIGHT);
    35          return TRUE;
    36  }

//======================

 1  gint nextbrickarea_configure (GtkWidget *widget, GdkEventConfigure *event)
     2  {
     3       if (nextbrick_pixmap)
     4              gdk_pixmap_unref(nextbrick_pixmap);
     5       nextbrick_pixmap = gdk_pixmap_new(widget->window, GAMEAREAWIDTH,
     6                                   GAMEAREAHEIGHT, -1);
     7       gdk_draw_rectangle (nextbrick_pixmap,  widget->style->white_gc, TRUE,  0, 0,
     8                               NEXTAREAWIDTH, NEXTAREAHEIGHT);                                  
     9       gdk_draw_pixmap(widget->window, widget->style->white_gc, nextbrick_pixmap, 0 ,0, 0, 0,
    10                        NEXTAREAWIDTH, NEXTAREAHEIGHT);
    11       return TRUE;
    12  }


//=====================
1  gint  gamearea_expose (GtkWidget *widget, GdkEventExpose *event)
     2  {
     3
     4      gint x,y;
     5      gint srcx, srcy, destx, desty;
     6      if(FALSE==bStop)
     7      {
     8         srcx=event->area.x/BLOCKWIDTH;
     9         srcy=event->area.y/BLOCKHEIGHT;
    10         destx=(event->area.x+event->area.width-1)/BLOCKWIDTH;
    11         desty=(event->area.y+event->area.height-1)/BLOCKHEIGHT;
    12         for(y=srcy;y<=desty;y++){
    13             for(x=srcx;x<=destx;x++){
    14                 if(g_Filled[x][y]==1){               /*方块占据区域*/
    15                     gdk_draw_rectangle (game_pixmap,gc, TRUE,
    16                                       x*BLOCKWIDTH,y*BLOCKHEIGHT,
    17                                       BLOCKWIDTH, BLOCKHEIGHT);
    18                     gdk_draw_rectangle (game_pixmap,gc1, FALSE,
    19                                       x*BLOCKWIDTH,y*BLOCKHEIGHT,
    20                                       BLOCKWIDTH, BLOCKHEIGHT);
    21                  }
    22                 else{
    23                       gdk_draw_rectangle (game_pixmap,widget->style->white_gc, TRUE,
    24                                             x*BLOCKWIDTH,y*BLOCKHEIGHT,
    25                                             BLOCKWIDTH, BLOCKHEIGHT);
    26                }
    27            }
    28         }
    29      }
    30      else
    31          gdk_draw_rectangle (game_pixmap,  widget->style->white_gc, TRUE,  0, 0,
    32                            GAMEAREAWIDTH, GAMEAREAHEIGHT);
    33     gdk_draw_pixmap(widget->window, widget->style->white_gc, game_pixmap,
    34                         event->area.x, event->area.y, event->area.x, event->area.y,
    35                         event->area.width, event->area.height);
    36
    37     return FALSE;
    38  }

//==============
 1  gint  nextbirckarea_expose (GtkWidget *widget, GdkEventExpose *event)
     2  {
     3        gint g_x=2,g_y=2,x1,y1,index;
     4        extern Brick g_nextbrick;
     5
     6          if(FALSE==bStop) {
     7               gdk_draw_rectangle (nextbrick_pixmap,  widget->style->white_gc, TRUE,  0, 0,
     8                                   NEXTAREAWIDTH, NEXTAREAHEIGHT);
     9              for(index=0;index<4;index++)
    10              {
    11                  x1=(g_nextbrick.brick[0].blockpos[index].x+g_x)*BLOCKWIDTH;
    12                  y1=(g_nextbrick.brick[0].blockpos[index].y+g_y)*BLOCKHEIGHT;
    13                  gdk_draw_rectangle (nextbrick_pixmap,gc, TRUE, x1,y1,
    14                                          BLOCKWIDTH, BLOCKHEIGHT);
    15                  gdk_draw_rectangle (nextbrick_pixmap,gc1, FALSE, x1,y1,
    16                                          BLOCKWIDTH, BLOCKHEIGHT);
    17              }
    18          }
    19          else
    20             gdk_draw_rectangle (nextbrick_pixmap,  widget->style->white_gc, TRUE,  0, 0,
    21                                        NEXTAREAWIDTH, NEXTAREAHEIGHT);
    22         gdk_draw_pixmap(widget->window, widget->style->white_gc, nextbrick_pixmap,
    23                                event->area.x, event->area.y, event->area.x, event->area.y,
    24                                event->area.width, event->area.height);
    25       return FALSE;
    26  }


