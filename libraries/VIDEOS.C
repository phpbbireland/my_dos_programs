#include <conio.h>
#include <videos.h>
#include <string.h>
#include <dos.h>
#include <xwindows.h>


#ifndef VGAP0
#define VGAP0  0xB8000000L;
//IBM8514LO
#endif

extern struct text_info w;

void WriteVideo(int w_m,int ssx, int ssy, char *line, int nattr, int fillupline)
{
	int far *farvideoptr;
	int p = 0;
	int nlen = 0;
	int frst = 0;
	int nattrold = 0;
	int token = 0;
	unsigned int cent = 0;
	int set = 0;
	int addup = 0;

//	int tolong = 0;
	farvideoptr = (int far *)VGAP0 /* colour address */

	//disable();

//****************************** count length ******************************//
	nlen = strlen(line);


if(w_m == 1 || w_m == 3)
if(nlen-2 > win->wr-win->wl)//added 22 nov 2000 to fix menu highlight problem
{
	line[win->wr-win->wl - 10] = '\0';
	strcat(line,"......");
	nlen = strlen(line);

}
//**************************************************************************//

	nattr = nattr << 8;
	nattrold = nattr;

	for(p = 0; p < nlen; p++)     // count tokens //
	{
	  if((frst = line[p]) == '‘')
	  token++;
	}

	switch(w_m)
	{
		case 3:  ssx = ssx + win->wl; ssy = ssy + win->wt;
		cent = (((win->wr)-(win->wl)) / 2) - ((nlen-token) / 2);	 break;
		case 2:  cent = 40 - ((nlen -token) / 2);			  			 break;
		case 1:  ssx = ssx + win->wl; ssy = ssy + win->wt;        break;
		case 10:  ssx++; ssy++;	break;
		default: break;
	}
	ssx--;
	ssy--;
	ssy = (ssy*80);
   addup = ssx+ssy+cent;

//**************************************************************************//

	for(p = 0; p < nlen ; p++)
	{

		if((frst = line[p]) == '‘')	
		{	
         frst = line[++p];
			if(frst > 47 && frst < 58)    // 1 to 9
		  	{
            p++;
				frst -= 48;	frst += 0x0;	
            nattr = (nattr & 0xf000) + (frst << 8); 
		  	}
		  	else
		  	if(frst > 96 && frst < 103)   // a to f
		  	{
            p++;
				frst -= 96;	frst += 0x9;	
            nattr = (nattr & 0xf000) + (frst << 8); 
		  	}
         else
         {
				if(frst == '‘')
				  p++;
				nattr = nattrold;
			}

         token += 2;
    	}


    	if((int)line[p] == 10) break; //added march 96 clean away the \r\n//

  		if((p-token) >= fillupline && fillupline != 0) 
		  nlen = fillupline;

		*(farvideoptr+addup+set) = ((int)line[p]) + nattr;
		set++;
//		if(debug = 1) delay(5);//test nov 2000//
	}
	 //enable();
//**************************************************************************//
//cprintf("fillupline = %d  Set =%d",fillupline,set);
	if(fillupline >= 0)
	{
		//disable();
		while(set < fillupline)
		{
			*(farvideoptr+ssx+set+ssy) = 255 + nattr; set++;
			nlen++;
		}
		//enable();
	}

}


void FillVideo(int left, int top, int right, int bottom, int atrib)
{
	int far *farptr;
  int leftt = left;

	farptr = (int far *)VGAP0

	//disable();
	while(top < bottom)
	{
		while( left < right)
		{
		  *(farptr + top*80+ left) = atrib;
		  left++;
		}
    left = leftt;
		top++;
	}
	//enable();
}


void PoinTo(int sx,int sy, int attr)
{
	int far *farvideoptr;
	farvideoptr = (int far *)VGAP0 /* colour address */
	sx--;
	attr = attr << 8;
	sy = sy * 80;
	*(farvideoptr+sx+sy) = 16 + attr+256;
}



/*
void VideoLineCentered(int wn,int y,char *i,int cr)
{
	unsigned int centre;
	if(wn == 1)
	 centre = (((win->wr)-(win->wl)) / 2);
	else
	centre = 40;

	WriteVideo(wn,(centre - (strlen(i) / 2)) ,y,i,cr,0);
}
*/


void WriteChar(char ch)
{
	if(ch == 10)
		return;

	if(ch == '\t')
		ch = ' ';

	_AH = 9;
	_AL = ch;
	_BH = 0;                  /* video page */
	_BL = w.attribute;          /* video attribute */
	_CX = 1;                  /* repetition factor */
	geninterrupt(0x10);       /* output the char */
}

