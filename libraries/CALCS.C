#include <stdio.h>                           
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <interupt.h>
#include <dir.h>
#include <process.h>
#include <keyios.h>
#include <xwindows.h>
#include <cursors.h>
#include <videos.h>
#include <editors.h>


struct text_info w,ti;
struct xwindows *win;//, *wincopy;


void OpenWin(int sx, int sy, int ex, int ey);
void CloseWin(int sx, int sy, int ex, int ey);

void Close(int sx, int sy, int ex, int ey);
void Move(void);
void swapchar(void);

int icon = 3;
int sx = 10, sy = 5, ex = 48, ey = 20;
int bufsize = 0;
char *winbuf;

void main()
{
	char buf1[30] = "";
	char *ep;
  long l1 = 0L;
//  long l2 = 0L;
  long answer = 0L;
  int code = 0;
	int a = 0,k = 0;
	int moved = 1;
	int offset = 0;

	k = 'C';
	a = 0;

//	CursorOff();


	do
	{
		if(moved) { Move(); moved = 0; }
		gotoxy(ex-sx-2,sy-3);

		do
		{
			if(k >= '0' && k <= '9' || k == '.')
			{
				buf1[a++] = k;
				offset = strlen(buf1)-1;
				gotoxy(ex-sx-2-offset,sy-3);
				cprintf("%s",buf1);
			}
			code = k = GetKey();

			if(k == '+' || k == '-' || k == '*' || k == '=' || k == 'C' || k == 'c')
			{
				k = CR;
				WriteVideo(0, sx+3,sy+ 1,"�f                                0",0x3f,0);
			}

			if(k == ESC) Close(sx,sy,ex,ey);

		}while(k != CR);

		l1 = strtol(buf1,&ep,10);

		switch(code)
		{
			case LEFT:  if(sx > 1)  { CloseWin(sx,sy,ex,ey); sx--; ex--; moved = 1; } break;
			case RIGHT: if(ex < 80) { CloseWin(sx,sy,ex,ey); sx++; ex++; moved = 1; } break;
			case UP:    if(sy > 1)  { CloseWin(sx,sy,ex,ey); sy--; ey--; moved = 1; } break;
			case DOWN:  if(ey < 25) { CloseWin(sx,sy,ex,ey); sy++; ey++; moved = 1; } break;

			case '-':   WriteVideo(0, sx+3,sy+ 1,"�f                                0",0x3f,0);
									answer = answer - l1;
									l1 = 0L;
									strset(buf1,'\0');
									memset(buf1,'\0',strlen(buf1));
									a = 0;
									moved = 0;
									k = 0;
									break;


			case '+':   WriteVideo(0, sx+3,sy+ 1,"�f                                0",0x3f,0);
									answer = answer + l1;
									l1 = 0L;
									strset(buf1,'\0');
									memset(buf1,'\0',strlen(buf1));
									ltoa(answer,buf1,10);
									a = strlen(buf1);
//									a = 0;
									moved = 0;
									k = 0;
									break;

			case 'C':
			case 'c':   WriteVideo(0, sx+3,sy+ 1,"�f                                0",0x3f,0);
									strset(buf1,'\0');
									memset(buf1,'\0',strlen(buf1));
									l1 = 0L;
									moved = 0;
									a = 0;
									k = 0;
									break;

			default:    break;
		}



		if(k == CR || k == '=')
    {
			gotoxy(ex-sx-2-offset,sy-3);
			cprintf("%ld",answer);
			answer = l1;
//			strset(buf1,'\0'); code = k; a = 0; memset(buf1,'\0',strlen(buf1));
			offset = strlen(buf1)-1;
			moved = 0;
		}

		if(k == ESC)
		{
			CloseWin(sx,sy,ex,ey);
//			CursorOn();
      return;
    }
	}while(k != ESC);

	CloseWin(sx,sy,ex,ey);
//	CursorOn();
}

void Move(void)
{
	OpenWin(sx,sy,ex,ey);
//	WriteVideo(0, sx+1,sy,"�f�������������������������������������Ŀ",0x1f,0);

	WriteVideo(0, sx+1,sy+ 0,"�f�����������������������������������Ŀ",0x1f,0);
	WriteVideo(0, sx+1,sy+ 1,"�f�                                   �",0x1f,0);
	WriteVideo(0, sx+1,sy+ 2,"�f�������������������������������������",0x13,0);
	WriteVideo(0, sx+1,sy+ 3,"�d�����������������������������������Ŀ",0x1f,0);
	WriteVideo(0, sx+1,sy+ 4,"�d�HEX�BIN�DEC�COS�TAN�SIN�LOG�1/X�OFF�",0x1f,0);
	WriteVideo(0, sx+1,sy+ 5,"�d�����������������������������������Ĵ",0x1f,0);
	WriteVideo(0, sx+1,sy+ 6,"�d�EXP� Yx� � � x2� ( � ) �LOG�   � | �",0x1f,0);
	WriteVideo(0, sx+1,sy+ 7,"�d�����������������������������������Ĵ",0x1f,0);
	WriteVideo(0, sx+1,sy+ 8,"�d� 1 � 2 � 3 � / � X - M �1/X�   � & �",0x1f,0);
	WriteVideo(0, sx+1,sy+ 9,"�d�����������������������������������Ĵ",0x1f,0);
	WriteVideo(0, sx+1,sy+10,"�d� 7 � 8 � 9 � * � R   M �   �   � ^ �",0x1f,0);
	WriteVideo(0, sx+1,sy+11,"�d�����������������������������������Ĵ",0x1f,0);
	WriteVideo(0, sx+1,sy+12,"�d� 4 � 5 � 6 � - � M   + �   �   � C �",0x1f,0);
	WriteVideo(0, sx+1,sy+13,"�d�����������������������������������Ĵ",0x1f,0);
	WriteVideo(0, sx+1,sy+14,"�d� 0 � � � . � + �       �   �   � = �",0x1f,0);
	WriteVideo(0, sx+1,sy+15,"�d�������������������������������������",0x13,0);
	WriteVideo(0, sx+3,sy+ 1,"�f                                0",0x3f,0);

}

void Close(sx,sy,ex,ey)
{
	CloseWin(sx,sy,ex,ey);
//	CursorOn();
	exit(0);
}

void OpenWin(int sx, int sy, int ex, int ey)
{
	bufsize = 2*(ex - sx + 1) * (ex - sx + 1);

	if((winbuf = (char *) malloc(bufsize)) == NULL)
	{ cprintf(" ERROR 5"); exit(2); }

	if(!gettext(sx,sy,ex,ey,(char *)winbuf))
	{ cprintf(" ERROR 3"); exit(3);	}

	window(sx,sy,ex,ey);
}

void CloseWin(int sx, int sy, int ex, int ey)
{
	puttext(sx,sy,ex,ey,(char *)winbuf);
	window(1,1,80,25);
}