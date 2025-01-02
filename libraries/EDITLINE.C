//   modded 131095 19:37 //

#include <keys.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>

#define FALSE 0

unsigned int shortcursor = 0x0607;
unsigned int tallcursor = 0x0107;
unsigned int oldcursor;

//void showdata(int,int);

int editline(char *s,char *legal,int maxlength, int attr,int edit)
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

//    showdata(pos,len);

		switch(c=GetKey())
		{

			case '\b': 
			if(pos > x -1)
			{
				xx = wherex();
				gotoxy(len+x-1,y); cputs(" "); //cprintf("%d",len);
				gotoxy(xx,y);
				movmem(&s[pos-x + 1], &s[pos -x ], len - pos+x+1); len--; pos--;
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
			insert = !insert;
			changecursor(insert);
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
			if(isprint(c) && pos-x+1 <= len)
			{ 
//				if (((legal[0] == 0) || (strchr(legal, c) != NULL)) && ((c >= ' ') && (c <= '~')) && (len < maxlength))

				if (((legal[0] == 0) || (strchr(legal, c) != NULL)) && ((c >= ' ') && (c <= '~')) && (pos-x < maxlength))
				{
					if (insert)
					{
          if(len > maxlength)
          {
            beep(100,200);
            break;
          }

						memmove(&s[pos - x + 2 ], &s[pos - x + 1], len - pos + 1 + x ); len++;
					}
					else if (pos-x+1 >= len)
					len++;
					s[(pos++)-x+1] = c;
				}
				else
				beep(100,200);
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

	}while((c != CR) && (c != ESC));
	changecursor(FALSE);
	return(c);
}


void main(void)
{

	char mybuffer[256] = "Michael Toole 12345";
	char legal[] = "";
	textbackground(CYAN);
	clrscr();
	gotoxy(5,5);
	editline(mybuffer,legal,20,0x1f,1);
	cprintf("\n\rResulting = [%s]",mybuffer);
	gotoxy(1,10);

	editline(mybuffer,legal,20,0x1f,0);
	cprintf("\n\r[%s]\n\r",mybuffer);
	gotoxy(1,15);
	strcpy(mybuffer,"Testing          OK");
	editline(mybuffer,legal,50,0x3e,1);
	cprintf("\n\rResulting = [%s]",mybuffer);
	GetKey();
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
