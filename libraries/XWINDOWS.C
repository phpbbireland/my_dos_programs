// all work such as puts() is done in curent window? No need to build more //
// function when they are not needed just call activate window if needed   //
/////////////////////////////////////////////////////////////////////////////
//                             0x????....                                  //
//  the colr variable = windowcolor-textcolor,borderbackground,bordercolor //
//  the type variable = icon-shaddow-2lineborder-2lineborder delay??       //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include <alloc.h>
#include <conio.h>
#include <xwindows.h>
#include <mouses.h>

#include <dos.h>
#include <string.h>
#include <keyios.h>
#include <process.h>
#include <string.h>
#include <stdio.h>
#include <cursors.h>
#include <sounds.h>
#include <videos.h>

#define SHADDOW 0x08ff//0x7fff

static int far *farvga0;
static int z = 0, i = 0, temp = 0;

void OpenWindow(int Left,int Top,int Right,int Bottom, char *Name,int Colour, int Type)
{
//	int wincolor,winnamecolor,wintextcolor,winbordercoor;
	int bufsize = 0, nlen = 0;

	int thistype = (Type & 0x00ff);

	farvga0 = (int far *)VGAP0;

	if(Type & 0x0004) z = 1;
	else z = 0;
	MouseOff();
	win = (struct xwindows *) malloc(sizeof(struct xwindows));

	bufsize = 2*(Bottom - Top + 1 + z) * (Right - Left + 1 + z + z);

	if((win->win_save_buff = (char *) malloc(bufsize)) == NULL)           { textcolor(WHITE); cprintf(" ERROR 5"); GetKey();exit(5);	}

	if(!gettext(Left,Top,Right+z+z,Bottom+z,(char *)win->win_save_buff))  { textcolor(WHITE);cprintf(" ERROR 3"); GetKey();exit(3);	}


	Win_Active_No = Win_Open_Count++; // the last open window must be the active window //
	windowpointers[Win_Open_Count].p = NULL;   // clear winpointer
	windowpointers[Win_Open_Count].status = 1; // tell last win it is not active

	(windowpointers[Win_Open_Count].p) = win;

	Left--;Top--;Right--;Bottom--;

	gettextinfo(&w);
	win->l = w.winleft;
	win->t = w.wintop;
	win->r = w.winright;
	win->b = w.winbottom;
	win->tcol = w.attribute;
	win->bcol = w.normattr;
	win->oldx = wherex();
	win->oldy = wherey();

	win->wl = Left;
	win->wt = Top;
	win->wr = Right;
	win->wb = Bottom;
	win->wc = Colour;
	win->title = Name;
	win->curx	= wherex();
	win->cury = wherey();
	win->type = Type;
	win->status_flag = 1;
	// status_flag: 0 == closed, 1 == Alive, 2 == Hidden //2002

	textcolor((Colour & 0x0f00) >> 8);       //window text colour
	textbackground((Colour & 0xf000) >> 12); //window back colour


	if((thistype >> 4) > 0)
		WindowEffect();

	if(thistype | 0x01 || thistype | 0x02)
		WindowBorder();

	if(thistype & 0x08)
		WindowIcon();

	if(thistype & 0x04)
		WindowShaddow();

	if(((nlen = strlen(Name))) <= (Right - Left))
	{
		temp = Left;
		temp = ((((Right-temp)/2)-(nlen/2))+(Top*80)) + Left;//??//

		for(i = 0; i < nlen; i++)
			*(farvga0+temp+i) = (int)Name[i] +  ((Colour & 0x00ff) << 8);
	}
	window(Left+2,Top+2,Right,Bottom);
	clrscr();
	MouseOn();
}


void CloseWindow(void)
{
	int syn = 0;

	win = windowpointers[Win_Open_Count].p;

	if(win->status_flag == 1) // if open
	{
		MouseOff();
		if(win->type & 0x0004) syn = 1;

		puttext(win->wl+1,win->wt+1,win->wr+1+syn+syn,win->wb+1+syn,(char *)win->win_save_buff);

		window(win->l,win->t,win->r,win->b);

		textcolor(win->tcol);
		textbackground(win->tcol>>4);

		gotoxy(win->oldx,win->oldy);

		free(win);
		free(win->win_save_buff);

//		free(win->win_save_data);

		Win_Open_Count--;
		win = windowpointers[Win_Open_Count].p;
		MouseOn();
	}
}

void WindowBorder(void)
{
 int i = 0;
 int colr = 0;
 int HOR,VER,NE,NW,SE,SW;

 colr = win->type & 0xff00;

 if(win->type & 0x0002) i = 2; else if(win->type & 0x0001) i = 1; //else i = 0;

	switch(i)
	{
		case 1: NW  = 0xda+colr; NE  = 0xbf+colr; 
						SW  = 0xc0+colr; SE  = 0xd9+colr;
						VER = 0xb3+colr; HOR = 0xc4+colr; break;

		case 2: NW  = 0xc9+colr; NE  = 0xbb+colr;
						SW  = 0xc8+colr; SE  = 0xbc+colr; 
						VER = 0xba+colr; HOR = 0xcd+colr; break;

		default: VER = HOR = NE = NW = SE = SW = (' '+colr); break;
	}

	*(farvga0 + (win->wl + win->wt * 80)) = NW; 
	*(farvga0 + (win->wr + win->wt * 80)) = NE;

	for(i = win->wt + 1; i < win->wb; i++)       
	{
		*(farvga0 + (win->wl + (i*80))) = *(farvga0 + (win->wr + (i*80)))= VER;
	}

	*(farvga0 + (win->wl + win->wb * 80)) = SW;
	*(farvga0 + (win->wr + win->wb * 80)) = SE;

	for(i = win->wl + 1; i < win->wr; i++)
	{
		*(farvga0 + (i + (win->wt * 80))) = *(farvga0 + (i + (win->wb * 80))) = HOR;
	}
}

void WindowShaddow(void)
{
  for(i = win->wl+2; i < win->wr + 3; i++)
	{
		if(win->wb < 25)
			*(farvga0 + (win->wb+1) * 80 + i) = (*(farvga0 + (win->wb+1) * 80 + i) & SHADDOW);
	}
	for(i = win->wt+1; i < win->wb + 1; i++)
	{ 
		if(win->wr < 80)
				*(farvga0 + i*80+ win->wr +1) = (*(farvga0 + i*80 + win->wr +1) & SHADDOW);

		if(win->wr < 79)
				*(farvga0 + i*80+ win->wr +2) = (*(farvga0 + i*80 + win->wr +2) & SHADDOW);
	}
}


void WindowIcon(void)
{
	*(farvga0 + (win->wl + 2 + win->wt * 80))   =  91 | (win->type & 0xff00);
	*(farvga0 + (win->wl + 2 + win->wt * 80)+1) =  Window_Icon | (win->type & 0xf000);
	*(farvga0 + (win->wl + 2 + win->wt * 80)+2) =  93 | (win->type & 0xff00);
}

void WindowEffect(void)
{
	int xleft,xtop,xright,xbottom;
	int ratio = 0, dly = 0;

	dly = ((win->type & 0x00f0) >> 4) * 5;

	ratio = ((win->wr - win->wl) / (win->wb - win->wt));
	ratio = 1; // ? //

	xleft = ((win->wr-win->wl)/2)+win->wl;
	xright = xleft;

	xtop = ((win->wb-win->wt)/2)+win->wt;
	xbottom = xtop;

	do
	{
		if(xleft  - ratio > win->wl) xleft  = xleft  - ratio;
		else
		xleft = win->wl;

		if(xright + ratio < win->wr) xright = xright + ratio; 
		else 
		xright = win->wr;

		if(xtop > win->wt) xtop--;
		if(xbottom < win->wb) xbottom++;
		window(xleft+2,xtop+2,xright,xbottom);
		clrscr();
		delay(dly);
//		WindowBorder();//??//

	}while(xleft !=  win->wl || xtop != win->wt || xright != win->wr || xbottom != win->wb);

}

void CloseAllWindows(void)
{
	while(Win_Open_Count > 1);//closeprocess closes its own window//dec96//
}

int SwitchWindow(int windo)
{
	if(Win_Open_Count <= 0)
		return(0);
	win = windowpointers[windo].p;

	window(win->wl, win->wt, win->wr, win->wb);
	return(windo);
}

void SetPage(int x,int y)     // move cursor to x,y on window page
{
	x+=1; y+=1;
}
void HideWindow()      // hide the window dont kill
{
	int syn = 0; // syn is ident for shaddow //

	win = windowpointers[Win_Open_Count].p; // get window pointer //

	if(win->status_flag == 1) // is it actually open //
	{
		MouseOff();
		if(win->type & 0x0004) syn = 1;

		puttext(win->wl+1,win->wt+1,win->wr+1+syn+syn,win->wb+1+syn,(char *)win->win_save_buff);

		window(win->l,win->t,win->r,win->b);

		textcolor(win->tcol);
		textbackground(win->tcol>>4);

		gotoxy(win->oldx,win->oldy);
		//***// remember to mod Win_Open_Count when window is re-activated / unhide
		Win_Open_Count--;
		win = windowpointers[Win_Open_Count].p;
		MouseOn();
}
void ActivateWindow(int thisone)  // bring to top
{
;
	// We need to activate thiswin
	//
	//
/*
	int caw;
	caw = Win_Active_No; //

	win = windowpointers[Win_Active_No].p;

	if(win->status_flag == Win_Active_No) // if Not Dead //
	{
		//get current xwindow //
	}
*/
}
void UnhideWindow(int thisone)    //
{
	Win_Open_Count++;
	// draw windos using structure //
}
void RenameWindow(char *name) //
{
/*
	win->title
*/
}
void MoveWindow(void)     // use left up down right keys until esc
{
;
}
void ResizeWindow(void)   // tab selects corner then arrow keys unyil esc
{
;
}
void CopyWindow(void)     //
{
;
}
