#include <dos.h>
#include <stdarg.h>
#include <keyios.h>
#include <stdio.h>
#include <string.h>
#include <mem.h>
#include <conio.h>
#include <cursors.h>
#include <xwindows.h>
#include <videos.h>

#define MAXWIDTH 255
#define MAXDEPTH 100

unsigned int  Window_Count;
unsigned int  Mouse_Detected;
unsigned int  Window_Icon;
unsigned int  Window_Index;
unsigned int  Error_Code;

unsigned Mouse_X,Mouse_Y;
unsigned Mouse_Lastx, Mouse_Lasty;
unsigned Mouse_Buttons;

struct wininfo windowpointers[MAXWINP];
struct text_info w;
struct xwindows *win;//, *wincopy;

void writechar(char ch);
void AddLine(int line, int x);
void FillTop(int line);
void FillBottom(int line);

void FillCharRight(void);
void FillCharLeft(void);
void scroll(int direction, int slines, int attr);

char stats[81];

char M[MAXDEPTH][MAXWIDTH] = {""}; 

int temp = 0;


int wsx =  2; 
int wsy = 10;
int wex = 63;
int wey = 22;

int i = 0;
int x = 0;

static temp_x;
static temp_y;
static current_x;
static current_y;
static current_t;
static current_b;
static current_l;
static current_c;

struct text_info ti;

void main(void)
{
  int rx,ry,rl;
  rx = wex-wsx;
  ry = wey-wsy+1;

	nosound();

  OpenWindow(wsx,wsy,wex,wey,"[ Read File ]",0x1f1f,0x1f06);

  current_t = 0;
  current_b = 9;

  current_x = wherex();
  current_y = wherey();

  for(i = 0; i < wey-wsy-1; i++)
  {  
    AddLine(i,1); 
  }
  gotoxy(1,1);

  while(i != ESC)
  {
    current_x = wherex();
    current_y = wherey();


/*
    WriteVideo(0,1,1," ",0x4f,80);
    sprintf(stats," X = %d Y = %d Current Line = %d Current Char = %d CurrentTop = %d Length %d",current_x,current_y,current_l,current_c,current_t,strlen(M[current_l]));
    WriteVideo(2,1,1,stats,0x4f,0);

*/
		sprintf(stats," Line [%d] Column [%d] ",current_l+1,current_x + current_c);
  	rl = strlen(stats);

  	WriteVideo(1,rx-rl-1,ry,"อออ",0x1f+0x01,0);
  	WriteVideo(1,rx-rl  ,ry,stats,0x1b,0);

  	CursorOn();
    i = GetKey();
    CursorOff();
    switch(i)
    {
      case UP:    if(current_y ==  1 && current_l > 0) 
                  {
									  temp_x = wherex();
									  temp_y = wherey();

                    scroll(DOWN,1,0x1f);  
                    current_t--;
                    FillTop(--current_l);
                    gotoxy(temp_x,current_y);
                  }
                  else 
                  {
                    if(current_l > 0) 
                    {
                    	gotoxy(current_x,--current_y); 
											current_l--;
                    }
                  }
                  break;

      case DOWN:  if(current_y == wey-wsy-1 && current_l < MAXDEPTH - 1) 
                  {
                    temp_x = wherex();
										temp_y = wherey();

                   	scroll(UP,1,0x1f);
                   	current_t++;
                   	FillBottom(++current_l);
                    gotoxy(temp_x,temp_y);
                  }
                  else 
                  {
									  if(current_l < MAXDEPTH) 
                    {
                      gotoxy(current_x,++current_y); 
										  current_l++;
                    }
                  }

                  break;

      case RIGHT: if(current_x == wex-wsx-1 && current_c < (MAXWIDTH-wex - wsx - 1)) // not at end
                  {
                    temp_y = wherey();
                    temp_x = wherex();

                    scroll(LEFT,1,0x1f); 

                    FillCharRight();

                    current_c++;

                    gotoxy(temp_x,temp_y);
                  }
                  else 
                  {
                    if(current_x < wex-wsx + 1)
									    gotoxy(++current_x,current_y);
                  }
                  break;

      case LEFT:  if(current_x ==  1 && current_c > 0)
                  {
                    temp_y = wherey();
                    temp_x = wherex();

                    scroll(RIGHT,1,0x1f);

                    FillCharLeft();

                    current_c--;

                    gotoxy(temp_x,temp_y);
                  }
                  else 
                  {
                    if(current_x > 1)
									  gotoxy(--current_x,current_y);
                  }
                  break;


      case END:   
			            if((current_x + current_c) < (strlen(M[current_l]))+1)
			            {  
									  StuffKey(RIGHT);
                    StuffKey(END);
                  }
  								break;

      case HOME:  
			            if((current_x + current_c) > 1)
			            {  
									  StuffKey(LEFT);
                    StuffKey(HOME);
                  }
									gotoxy(1,current_y);  
									break;

      default: break;
    }
  }
  CloseWindow();
  clrscr();
}

void scroll(int direction, int slines, int attr)
{

int p = 0;
  int sx,ex,sy,ey;

  sx = win->wl; 
  sy = win->wt; 
  ex = win->wr; 
  ey = win->wb;

  sx = sx + 2; sy = sy + 2;
  ex = ex; ey = ey;

  textbackground(WHITE);
  window(sx,sy,ex,ey);

  if (slines == 0)
    window(sx, sy, ex, ey);

  else switch(direction)
  {
    case UP :
      p = movetext(sx, sy + slines, ex, ey, sx, sy);
      window(sx+1, (ey - slines)+2, ex, ey-1);
      break;
    case DOWN :
      p = movetext(sx, sy, ex, ey - slines, sx, sy + slines);
      window(sx, sy, ex, sy + slines - 1);
      break;
    case LEFT :
      p = movetext(sx + slines, sy, ex, ey, sx, sy);
      window(ex - slines + 1, sy, ex, ey);
      break;
    case RIGHT :
      p = movetext(sx, sy, ex - slines, ey, sx + slines, sy);
      window(sx, sy, sx + slines - 1, ey);
      break;
  }

  if(p == 0)
    Beep(1000,500);


  textcolor(attr);
  textbackground(attr >> 4);

  window(sx,sy,ex,ey);
//  delay(500);
} 

void FillCharRight(void)
{
  int x = current_x, y = 1;
  int ii = 0;

  gettextinfo(&ti);

  textcolor(YELLOW);

  gotoxy(x,y);

  for(ii = current_t; ii < wey-wsy-1 + current_t; ii++)
  {
    writechar(' ');
    writechar(M[ii][current_c+wex-wsx-1]);
    gotoxy(x, ++y);
  }
}

void FillCharLeft(void)
{
  int x = current_x, y = 1;

  int ii = 0;
  gettextinfo(&ti);

  gotoxy(1,y);

  for(ii = current_t; ii < wey-wsy-1 + current_t; ii++)
	{
    writechar(' ');
    writechar(M[ii][current_c-1]);
    gotoxy(x, ++y);
  }
}


void AddLine(int line, int startx)
{
  int ii = 0;
  gettextinfo(&ti);

  current_x = startx;

  gotoxy(startx, current_y);

  for(ii = current_c; ii < wex-wsx-1; ii++)
  {
    writechar(M[line][ii]);
    gotoxy(++current_x, current_y);
  }

  if(current_y < wey-wsy-1)
  	++current_y;
}

void FillBottom(int line)
{
  int x = 1,y = current_y;
  int ii = 0;
  gettextinfo(&ti);


  gotoxy(x, y);
  clreol();

  for(ii = current_c; ii < wex-wsx-1+current_c; ii++)
  {
    writechar(M[line][ii]);
    gotoxy(++x, y);
  }
}

void FillTop(int line)
{
  int x = 1, y = current_y;
  int ii = 0;
  gettextinfo(&ti);


  gotoxy(x,y);

  clreol();

  for(ii = current_c; ii < wex-wsx-1+current_c; ii++)
  {
    writechar(M[line][ii]);
    gotoxy(++x, y);
  }

}


void writechar(char ch)
{
   _AH = 9;
   _AL = ch;
   _BH = 0;                  /* video page */
   _BL = ti.attribute;       /* video attribute */
   _CX = 1;                  /* repetition factor */
   geninterrupt(0x10);       /* output the char */
}
