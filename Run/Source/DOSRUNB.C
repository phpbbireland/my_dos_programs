// Created July 96 To Backup Restore DOSRUN DATA FILE //
#include <io.h>
#include <dir.h>
#include <dos.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <conio.h>
#include <keyios.h>
#include <cursors.h>
#include <mouses.h>
#include <xwindows.h>
#include <editors.h>
#include <videos.h>
#include <sounds.h>


unsigned Window_Icon, Window_Count;

unsigned Mouse_X,Mouse_Y;
unsigned Mouse_Lastx, Mouse_Lasty;
unsigned Mouse_Buttons;
unsigned Mouse_Detected;


struct wininfo windowpointers[MAXWINP];
struct text_info w, ti;
struct xwindows *win;//, *wincopy;

int GlobalKey;

void main(void)
{
  int k = 0;
  OpenWindow(10,5,44,8,"",0x0f0f,0x0401);
  cprintf(" Press B to Backup. R to Restore");
  k = GetKey();
  CloseWindow();
  if(k == ESC)
		return;

	OpenWindow(2,5,77,12,"",0x0f0f,0x0401);

 	if(k == 'B' || k == 'b')
		system("Copy C:\\RUN\\DOSRUN.REC C:\\RUN\\DOSRUN.BAX");
  else
  if(k == 'R' || k == 'r')
  {
		cprintf("\n\r Making Temp Copy C:\\RUN\\DOSRUN.BX2");
		system("COPY C:\\DOS\\DOSRUN.REC C:\\RUN\\DOSRUN.BX2");
		cprintf("\n\r Deleting C:\RUN\\DOSRUN.REC");
		system("DEL  C:\\RUN\\DOSRUN.REC");
		cprintf("\n\r Renameing C:\\RUN\\DOSRUN.BAX DOSRUN.REC");
		system("REN  C:\\RUN\\DOSRUN.BAX DOSRUN.REC");
		cprintf("\n\r Renameing C:\\RUN\\DOSRUN.BA2 DOSRUN.BAX");
		system("REN  C:\\RUN\\DOSRUN.BX2 DOSRUN.BAX");
	}
  CloseWindow();
}

void Update(void)
{;}
