//   modded 090595 10:00 //

#include <keys.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>

#define FALSE 0
void showdata(int,int);

int eline(char *s,char *illlegal,int maxlength, int attr,int edit)
{

	int c, len = strlen(s), pos = len, insert = FALSE;
	int x,y,xx; 

	x = wherex(); 
	y = wherey();

	textcolor(attr);
	textbackground(attr>>4);

	if(edit == 1) 
	{
		if(!strcmp(s," ")) 
			s[0] = '\0';
	}
	else
	{
		strnset(s,'\0',maxlength);
		len = strlen(s);
		pos = len;
	}
	pos = pos + x - 1;
	do
	{
		gotoxy(x,y);
		cprintf("%s",s);
		gotoxy(pos+1,y);

		switch(c=getkey())
		{

			case '\b': 
			if(pos > x -1)
			{
				xx = wherex();
				gotoxy(len+x-1,y); cputs(" "); //cprintf("%d",len);
				gotoxy(xx,y);
				movmem(&s[pos-x + 1], &s[pos -x ], len - pos+x+1); 
        len--; 
        pos--;
			}
			break;

			case LEFT: 
			if(pos > x-1)  // Left Arrow
			{
				pos--;
			}
			
			break;

			case RIGHT:
			if(pos < len+x-1)
				pos++;
			break;

			case HOME:
			pos = x - 1;//pos = 0;
      break;

			case END: 
			pos = len + x - 1; //pos = len;
			break;

			case INS: 
			break;

			case DEL: 
			if (pos < len+x-1)
			{
				xx = wherex();
				gotoxy(len+x-1,y); cputs(" "); //cprintf("%d",len);
				gotoxy(xx,y);
				movmem(&s[pos-x+1 + 1], &s[pos-x+1], len - pos+x+1); len--;
			}
			break;

			default:  

      if(strchr(illlegal, c) != NULL)
        return(c);

			if(isprint(c) && (pos-x+1 <= len ))
			{ 

				if (((illlegal[0] == 0) || (strchr(illlegal, c) == NULL)) && ((c >= ' ') && (c <= '~')) && (pos-x < maxlength))
				{
					if (insert)
					{
            beep(1000,600);
						memmove(&s[pos - x + 2 ], &s[pos - x + 1], len - pos + 1 + x ); len++;
					}
					else 
          if (pos-x+1 >= len)
					len++;

					s[(pos++)-x+1] = c;
			  }
				else
        
				beep(100,400);
				break;
			}
		}// end of switch

		if(c == ESC || c & 0x80) 
		{ 
			if(c != LEFT && c != RIGHT && c != HOME && c != END && c != INS && c != DEL)
			{
				return(c);
			}
		}
  showdata(pos,len);

	}while((c != CR) && (c != ESC));

	return(c);
}


void main(void)
{

	char mybuffer[256] = "Michael O'Toole 12345";
	char illlegal[] = " /+";
	textbackground(CYAN);
	clrscr();
	gotoxy(5,5);
	eline(mybuffer,illlegal,20,0x1f,1);
	cprintf("\n\rResulting = [%s]",mybuffer);
	gotoxy(1,10);

	eline(mybuffer,illlegal,50,0x1f,0);
	cprintf("\n\r[%s]\n\r",mybuffer);
	gotoxy(1,15);
	strcpy(mybuffer,"Testing          OK");
	eline(mybuffer,illlegal,50,0x3e,1);
	cprintf("\n\rResulting = [%s]",mybuffer);
	getkey();
}


void showdata(int z, int a)
{
 int x,y;
 x = wherex();
 y = wherey();

 gotoxy(1,1);
 clreol();
 cprintf("Position = %d Length = %d",z,a);
 gotoxy(x,y);

}