#include <conio.h>
#include <stdio.h>
#include <xwins.h>
//#include <mmouse.h>

extern int icon;
extern int error_code;

struct windo *win;

struct text_info w;

struct windo *xcreate(int l,int t,int r,int b,int bbattr, char *name,int nnattr,int bor,int sh,int typ)
{
	static int far *farprt;
  int NS,EW,NE,NW,SE,SW;  // corners //
	int lx,tx,rx,bx;

	int i = 0;
	int nlen = 0;
	int bufsize = 0;

	farprt = (int far *)VMODE; //0XB8000000;

//	mouseoff();
	gettextinfo(&w);

  if(sh > 0) sh = 1;

	win = (struct windo *) malloc(sizeof(struct windo)); // allocate space for win

	bufsize = 2*(b - t + 1 + sh) * (r - l + 1 + sh + sh);

	if((win->win_save_buff = (char *) malloc(bufsize)) == NULL)
	{
		cprintf("MEMALLERROR");
		getkey();
	}

	if(!gettext(l,t,r+sh+sh,b+sh,(char *)win->win_save_buff))
	{
		cprintf("GETTEXTERROR");
		getkey();
	}

	if(((nlen = strlen(name))) > (r - l))
	{
		cprintf("NAMETOLONG");
		getkey();
	}


	// save previous window info so you can restore ?? options are get the info //


	win->l = w.winleft;
  win->t = w.wintop; 
	win->r = w.winright;
	win->b = w.winbottom;
  win->na = w.attribute;
	win->ba = w.normattr;
	win->oldx = wherex();
	win->oldy = wherey();

	win->wleft   = l;
  win->wtop    = t;
  win->wright  = r;
  win->wbottom = b;
	win->nattr = nnattr;
	win->battr = bbattr;
	win->win_name = name;
	win->namesize = nlen;
	win->curx = wherex();
  win->cury = wherey(); 
	win->status_flag = 1;
	win->border  = bor;
	win->shaddow = sh;
	win->type    = typ;
	win->nattr = win->nattr<<4;
	win->nattr = win->nattr>>12;

  bbattr = bbattr << 8;
  nnattr = nnattr << 8;

	textcolor(nnattr >> 8);
	textbackground(nnattr >> 12);

	if(bor == 2) { NW =  0xc9; NE = 0xbb; SW = 0xc8; SE = 0xbc; EW = 0xba; NS = 0xcd; }
	else
	if(bor == 1) { NW =  0xda; NE = 0xbf; SW = 0xc0; SE = 0xd9; EW = 0xb3; NS = 0xc4; }
	else
	if(bor == 0) { NW = NE = SW =  SE = EW = NS = ' '; }

	hidecursor();

	typ = typ * 10;
	r = r -1;
	b = b -1;
	
	lx = ((r-l)/2)+l;
	rx = lx;
	tx = ((b-t)/2)+t;
	bx = tx;

	do
	{
		if(typ == 0){ lx = l; tx = t; rx = r; bx = b; }
		else
		{
			if(lx > l)lx--;
			if(tx > t)tx--;
			if(rx < r)rx++;
			if(bx < b)bx++;
			if(lx > l)lx--;
			if(rx < r)rx++;
			if(lx > l)lx--;
			if(rx < r)rx++;
		}

		for(i = lx; i < rx; i++)
		{ 
			*(farprt + (i + ((tx-1)*80))) = *(farprt + (i + (bx*80))) = NS +  bbattr ;
		}
		for(i = tx; i < bx; i++)
		{
			*(farprt + (lx-1 + (i*80))) = *(farprt + (rx + (i*80)))= EW + bbattr;
		}


		*(farprt + (lx-1 + ((tx-1)*80))) = NW + bbattr;
		*(farprt + (rx + (tx-1)*80)) = NE + bbattr;
		*(farprt + (lx-1 + (bx*80))) = SW + bbattr;
		*(farprt + (rx + bx*80)) = SE + bbattr;

														//  shaddow  //
	if(sh)
	{ 
		for(i = lx+1; i <= rx; i++)
		{
			if(bx < 25) *(farprt + (bx+1)*80+ i) = (*(farprt + (bx+1)*80 + i) & 0x08ff);
		}

		for(i = tx; i <= bx+1; i++)
		{ 
		if(r < 80)	
		*(farprt + i*80+ rx+1) = (*(farprt + i*80 + (rx+1)) & 0x08ff); 
		if(r < 79)
		*(farprt + i*80+ rx+2) = (*(farprt + i*80 + (rx+2)) & 0x08ff);
		}
	}
		// shaddow was here //
		window(lx+1,tx+1,rx,bx);
		clrscr();
		
		delay(typ);
	}while(lx !=  l || tx != t || rx != r || bx != b);

	unhidecursor();
 
	if(bor > 0 && sh >= 1)
	{ 
		l = l + 2;
		*(farprt + (l-1 + (tx-1)*80)) =  91 + bbattr;  // [
		*(farprt + (l   + (tx-1)*80)) = icon+ nnattr;//bbattr;  // shamrock 5
		*(farprt + (l+1 + (tx-1)*80)) =  93 + bbattr;  // ]
		l = l -2;
	}
															//  name  //
	lx = ((((r-l)/2)-(nlen/2-1))+(--t*80))+l;
	--lx;
	if(nlen/2 == 1) --lx; // if uneven move left //
	for(i = 0; i < nlen; i++)
		*(farprt+lx+i) = (int)name[i] + nnattr;
//		mouseon();
		return(win);
}


void xdelete(struct windo *win)
{
	int syn = 0;
	if(win->status_flag == 1) // if open
	{
//		mouseoff();
		syn = win->shaddow;

		puttext(win->wleft,win->wtop,win->wright+syn+syn,win->wbottom+syn,(char *)win->win_save_buff);
		free((void *)win->win_save_buff);

		win->status_flag = 0;
		window(win->l,win->t,win->r,win->b);

		textcolor(win->na);
		textbackground(win->na>>4);
		gotoxy(win->oldx,win->oldy);
//		mouseon();
		free(win);
	}
}
