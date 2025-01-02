#include <conio.h>
#include <xwins.h>

void to_vw(int startx, int starty, char *textline, int nattr, int opt)
{
	int far *farvideoptr;
	int sx,sy; 
	int count; 
	int nlen = 0;
	
	struct text_info t_info;

	farvideoptr = (int far *)VMODE /* colour address */

	gettextinfo(&t_info);  sx = t_info.winleft; sy = t_info.wintop;

	startx = startx + sx - 1;
	starty = starty + sy - 1;

	nattr = nattr << 8;
	
	nlen = strlen(textline);
	starty = (starty*80);


	for(count = 0; count < nlen; count++)
	{
		*(farvideoptr+startx+count+starty) = ((int)textline[count]) + nattr;
	}


	if(opt != 0)
	{
		while(count < opt)
		{
			*(farvideoptr+startx+count+starty) = 255 + nattr; count++;
		}
	}
}

void to_video(int startx, int starty, char *textline, int nattr, int opt)
{
	int far *farvideoptr;
	int nlen = 0;
	int i = 0;

	farvideoptr = (int far *)VMODE /* colour address */

	nattr = nattr << 8;
	
	nlen = strlen(textline);
	starty = (starty*80);

	startx = startx + starty;

	for(i = 0; i < nlen; i++)
	{
		*(farvideoptr+i+startx) = ((int)textline[i]) + nattr;
	}

	if(opt != 0)
	{

		while(i < opt)
		{
			*(farvideoptr+startx+i++) = 255 + nattr;
		}

	}
}
