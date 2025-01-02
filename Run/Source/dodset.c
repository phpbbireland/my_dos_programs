#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <process.h>

#include <xwindows.h>
#include <cursors.h>
#include <keyios.h>
#include <myerrors.h>
int GlobalKey;

/*					MACROS					*/
#define streql(x,y) (strcmp(x,y)==0)

#define TRUE  1
#define FALSE 0
#define FIRST 0
#define NEXT  1
#define AFILE 1
#define DIR   16

#define IGNORE  0
#define RETRY   1
#define ABORT   2

static int i;
static int k;

int originaldrive;
int far *farptr;
int error_code;

int totaldisks;
int get_all_disks(void);

char msg[200];

FILE *fileone;

unsigned int Window_Icon;
unsigned int Window_Count;
unsigned int Error_Code;
unsigned int Current_Page;
unsigned int Window_Action = 0;

struct wininfo windowpointers[MAXWINP];
struct text_info w;
struct xwindows *win;//, *wincopy;

unsigned Mouse_X,Mouse_Y;
unsigned Mouse_Lastx, Mouse_Lasty;
unsigned Mouse_Buttons;
unsigned Mouse_Detected;

void procede(void);

search(char *, int ,int);
search(char *fname, int flag, int type)
{
	union REGS regs;

	regs.h.ah = 0x4e + flag;	/* ah = 4E for find first & 4F for next */
	regs.x.cx = type;		/* cx = attribute to use ?? */
	regs.x.dx = (int)fname;
	intdos(&regs,&regs);
	if(regs.x.cflag == 1) return(FALSE);
	return(TRUE);

}

/*					SETUP DTA					*/
void set_dta(char *);
void set_dta(char *ptr)
{
	union REGS regs;
	regs.h.ah = 26;
	regs.x.dx = (int)ptr;
	intdos(&regs,&regs);
}

void depth_search(char *, char *);
void depth_search(char *dir, char *name)
{
	char dirname [256]; 
	char filename[256];
	char dta[43];

	int flag;
  int disk;
	static int oldx;
  static int oldy;

	disk = getdisk();
	sprintf(filename,"%s\\%s",dir,name);
	set_dta(dta);
	flag = FIRST;
	textcolor(WHITE);
  CursorOff();

	while(search(filename,flag,AFILE))
	{
		fprintf(fileone,"%c:\%s\\%s\n\r",disk+'A',dir,dta+30);//////////,
		gotoxy(1,9);
		clreol();
		cprintf(" %c:\%s\\%s",disk+'A',dir,dta+30);
		flag = NEXT;
	}
	gotoxy(oldx,oldy);

	sprintf(filename,"%s\\*.",dir);

	flag = FIRST;

	while(search(filename,flag,DIR))
	{
		flag = NEXT;
		if(!streql(".",dta+30) && !streql("..",dta+30))
		{
	    sprintf(dirname,"%s\\%s",dir,dta+30);
			depth_search(dirname,name);
		}
		set_dta(dta);
	}
	textcolor(YELLOW);
}

void main(argc,argv)int argc; char *argv[];
{

	char temp[81];
	char name[70];
//	char FileToFind[10] = "";
	int lk = 0;

	originaldrive = getdisk();

	totaldisks =  get_all_disks();

	strcpy(temp,strupr(argv[1]));

	if(strstr(temp,"MP3"))
	{
		lk = 1; // used to check which file type to look for //
	}
	else
	lk = 0;

	if(lk==1) sprintf(name,"[ Playlist Builder Version 1.00 - Searching... Drive(s) & CD(s) ]");
	else
	sprintf(name,"[ DosRun & Navigate Setup Utility [ Hard & CD Drives ]");
	OpenWindow(2,7,77,17,name,0x0f04,0x0405);
	CursorOn();
	gotoxy(1,8);

	harderr(handler); // ........

	textcolor(WHITE);
	gotoxy(1,2);
	cprintf("           DOSRUN, Advanced Setup (C) M.O'TOOLE 1995 Version 7.00\n\r");
	textcolor(YELLOW);

  //cprintf("OK = %s\n\r%s\n\r %d",temp,argv[1],lk); GetKey(); exit(0);

	if(lk==1)	fileone = fopen("C:\\DOS\\MP3S.M3U","wt+");
	else fileone = fopen("C:\\DOS\\DOSRUN.DAT","wt+");

	gotoxy(22,6);
	textcolor(LIGHTBLUE);
	if(lk)cprintf("Building: -> C:\\DOS\\MP3S.M3U");
	else cprintf("Building: -> C:\\DOS\\DOSRUN.DAT");

	textcolor(YELLOW);
	for(i = 2; i < totaldisks; i++)
	{
		procede();
		if(k == ESC)
			break;

		setdisk(i);
		gotoxy(1,4); clreol();

		if(lk==1)
		{
			cprintf("                 Looking For -> All .MP3 Files on Drive %c:",i+'A');
			depth_search("","*.MP3");
		}
		else
		{
			cprintf("                 Looking For -> All .EXE Files on Drive %c:",i+'A');
			depth_search("","*.exe");
		}
		gotoxy(1,4); clreol();
		if(lk==1)
		{
			cprintf("                  Looking For -> All .M3U Files on Drive %c:",i+'A');
			depth_search("","*.M3U");
		}
		else
		{
			cprintf("                  Looking For -> All .COM Files on Drive %c:",i+'A');
			depth_search("","*.com");
		}
		gotoxy(1,4); clreol();
		if(lk!=1)
		{
			cprintf("                 Looking For -> All .BAT Files on Drive %c:",i+'A');
			depth_search("","*.bat");
		}
	}

	CloseWindow();
	setdisk(originaldrive);
	CursorOn();
	fclose(fileone);
}

int get_all_disks(void)
{	
	int disk,save;

	save = getdisk();
  for (disk = 0; disk < 26; ++disk)
  {
      setdisk(disk);
      if (disk == getdisk())
      totaldisks++;
  }
	setdisk(save);
	return(totaldisks);
}

unsigned long cash = 230455L;
// error section //
error_win(char *msg)
{
   int retval;

	 OpenWindow(17,8,63,14,"[ Drive Error ] ",0x4f4f,0x4f0f);
   
   cputs(msg);

   while(1)
   {
       retval= getch();
       if (retval == 'a' || retval == 'A')
       {
           retval = ABORT;
           break;
       }
       if (retval == 'r' || retval == 'R')
       {
           retval = RETRY;
           break;
       }
//       if (retval == 'i' || retval == 'I')
//       {
//           retval = IGNORE;
//           break;
//       }
   }
	 CloseWindow();
   if(retval == ABORT)
   {
     setdisk(originaldrive);
		 CursorOn();
		 fclose(fileone);
		 CloseWindow();
   }
   return(retval);
}

#pragma warn -par
int handler(int errval,int ax,int bp,int si)
{
   static char msg[80];
   unsigned di;
   int drive;
   int errorno;

   di= _DI;

   if (ax < 0)
   {
      error_win("Device error");
      hardretn(ABORT);
   }

   drive = ax & 0x00FF;
   errorno = di & 0x00FF;

   sprintf(msg, "\n\r       %s on drive %c\n\r\n\r              A)bort, R)etry ", err_msg[errorno], 'A' + drive);

   hardresume(error_win(msg));
   return ABORT;
}
#pragma warn +par

void procede(void)
{
	k = 0;

	if(i > 2 && i <= totaldisks) // if drive C, D, E, etc //
	{
		OpenWindow(22,9,58,15," Additional Drives ",0x3f3f,0x3f0f);
		CursorOff();

		while(k != CR)
		{
			cprintf("\n\r     Press Enter to search %c:\n\r\n\r     Any other key to skip %c:",i+'A',i+'A');
			k =  GetKey();

			if(k == ESC) { clrscr(); ++i; }
			if(i >= totaldisks)
			{
				CloseWindow();
				return;
			}
		}
		CloseWindow();
	}
}

void Update(void){;}
