#include <conio.h>
#include <xwindows.h>
#include <keyios.h>
#include <videos.h>

void ScrollWindow(int direction, int number, int attr);
void ScrollWindow(int direction, int number, int attr)
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

	if (number == 0)
    window(sx, sy, ex, ey);

	else

	switch(direction)
  {
    case UP :
			p = movetext(sx, sy + number, ex, ey, sx, sy);
			window(sx+1, (ey - number)+2, ex, ey-1);
			break;
    case DOWN :
			p = movetext(sx, sy, ex, ey - number, sx, sy + number);
			window(sx, sy, ex, sy + number - 1);
      break;
    case LEFT :
			p = movetext(sx + number, sy, ex, ey, sx, sy);
			window(ex - number + 1, sy, ex, ey);
      break;
    case RIGHT :
			p = movetext(sx, sy, ex - number, ey, sx + number, sy);
			window(sx, sy, sx + number - 1, ey);
      break;
  }

	if(p == 0)
	{
		cprintf("Scroll Error");
		GetKey();
	}
	textcolor(attr);
  textbackground(attr >> 4);
	window(sx,sy,ex,ey);
}
