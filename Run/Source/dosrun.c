// Modified 20/11/96 6:40 P.M. //
// Copyright Michael O'Toole 1984-2025 //

#define NODEBUG    //use to disallow tc debug
#define SHAREWARE  //compile directive for shareware version
#define OPENSOURCE //compile directive for open source version 

#include <ctype.h>
#include <bios.h>
#include <io.h>
#include <dir.h>
#include <dos.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <process.h>
#include <share.h>
#include <mem.h>
#include <keyios.h>
#include <cursors.h>
#include <mouses.h>
#include <xwindows.h>
#include <editors.h>
#include <videos.h>
#include <sounds.h>
#include <menus.h>
#include <interupts.h>
#include <myerrors.h>
#include <indexes.h>
#include <files.h>

#include "dosrun.h"
#include "dosrun.men"
#include "dosrun.glo"
#include "dosrun.pro"

#define TRUE   1
#define FALSE  0
#define IGNORE 0
#define RETRY  1
#define ABORT  2
#define OK     1
#define NOTOK  0
#define FAULT -1
#define ERROR -1


#define  dosrunbat "C:\\RUN\\DOSRUNN.BAT"   //
#define  dosrunrec "C:\\RUN\\DOSRUN.REC"    //
#define  dosruntec "C:\\RUN\\DOSRUN.TEC"    //
#define  dosrunindex "C:\\RUN\\DOSRUNN.NDX" //
#define  dosrunhelp  "C:\\RUN\\DOSRUNH.HLP" //

static int tick;
static int INMENU;
static int mnth;
static int xt;

int GetJustification(int);
char WriteHelpPage(int);
char WriteHelpLine(int);


void WindowsOptions(int);

int wjust=0,xjust=0,yjust=0;
int screencolour = 0x7000;  //default grey//
int SW = 0;

char TOPLINE2[78] = "Licenced To: ";

FILE *doshelp,*txtfile;

struct text_info ti;
char *ThisLine = "";

void main(argc,argv)int argc; char *argv[];
{
//	int handle;
	char *stop = NULL;
	harderr(handler);

	Busy_Flag = 0;
	Message_Waiting = 0;

	if(strstr(argv[1],"LOGO"))
	{
		system("C:\\RUN\\LOGO /FAD");
	}

/*
		/// NEW ///
		handle = creatnew("C:\\RUN\\temp", 0);
		if(handle == -1) //if it exists //
			;
		else
		{
			close(handle);
			system("LOGO /FAD");
		}
	/// NEW ///
*/

	if((WaitForKey = atoi(argv[3])) == 1)
	{
		cprintf("\rPress Any Key");
		GetKey();
		cprintf("\r             \r\n");
	}
	WaitForKey = 0;
	ClearBiosBuff();
	SetKeys();
	Start();
	HookVectors();


//===========
	fd1 = OpenFile(dosrunrec,O_RDWR | O_CREAT | O_BINARY,SH_DENYNO,S_IWRITE);

//	fd1 = open(dosrunrec, O_RDWR | O_CREAT | O_BINARY,S_IWRITE );//create new
//	if(fd1 == -1)
//		CallCritError(1);
//===========


	ShowTimeTop();
	ShowDateTop();

	RunPrompt("",0x1e);

	Labels();

	if(argc == 4)
	{
		At_Page   = strtol(argv[1], (char **)stop, 10);
		At_Line = atoi(argv[2]);
	}

	if(argc == 1)
		lseek(fd1, Current_Record, SEEK_SET);
	else
		lseek(fd1, At_Page, SEEK_SET);

	OpenWindow(3,4,60,18," Programmes ",0x1f1b,0x1f06);
	paint_scrn_1();

	SeekBeginingCurRcd(fd1);
	NextRecord();

	ClearBiosBuff();

	if(mnth+256 >= fnd.dat+1 && SW)
	{  Rep("The Shareway Date has Expired..... ",0x4f+BLINK,1);
		 Rep("Re-Install Dosrun from its Original Disk......",0x4f+BLINK,1);
		 CallCritError(2);
	}

	Code_Flag = MainLoop(Per_Entry_Field);

	if(Launch_Flag == 1)
		OpenBatch(Code_Flag);

//===========
	CloseFile(fd1);
//	close(fd1);
//===========


	CloseWindow();
	CursorOn();

	// Execute A Programme //

	End();
	UnhookVectors();

	if(Launch_Flag == 1)
	{
		StuffKey(ESC);
		StuffBuffer("DOSRUNN");
		StuffKey(13);
		exit(0);
	}
	StuffKey(ESC);
	ClearBiosBuff();
} // main ends //

////////////////////////// screens & menus  ////////////////////////////

static void paint_scrn_1(void)
{
  for(i = 3; i < 13; i++)
	WriteVideo(1,3, i, ">",0x1b,0);//a = LG
  if(SW)
	{
		WriteVideo(0, 12, 16,"  (‘eSHAREWARE VERSION PLEASE REGISTER‘‘)  ",0x1b,0);
		WriteVideo(0, 13, 16,"*",0x1b+BLINK,0);
		WriteVideo(0, 49, 16,"*",0x1b+BLINK,0);
	}
}

void RunPrompt(char *n, int x)
{
  int cntr = 0;

	WriteVideo(0,1,25,"",x,80);

  if(x == (0x0c+BLINK))
    cntr = 2;

  if(strlen(n) == 0)
		WriteVideo(cntr,1,25," ‘bReport =>‘‘ Press Alt for Menus",0x1e,0);
  else
		WriteVideo(cntr,1,25,n,x,0);
}

static void ShowRecords(void)
{
	char rpt1[31];

	Record_Size  = sizeof(nd);
	File_Length  = filelength(fd1);
	Record_Count = (File_Length / Record_Size);

  sprintf(rpt1,"Menu [%ld] of [%d]",(Current_Record/Record_Size),(Record_Count));

	WriteVideo(0,62,25," ",0x1f,8);
	WriteVideo(0,79,25," ",0x1f,0);

	if(Current_Record/Record_Size < 10)
		WriteVideo(0,64,25,rpt1,0x1f,0);
	else

	if(Current_Record/Record_Size > 9)
		WriteVideo(0,63,25,rpt1,0x1f,0);
	else

	if(Current_Record/Record_Size > 99)
		WriteVideo(0,62,25,rpt1,0x1f,0);
}


void OpenBatch(num)
{
	char batbat[256];
	char drve[3] = "C:";
	char tmp[81] = "CALL ";
	int handle;
	char *itshere, *itshere2;
	num = num - 1;

	// do we want to pause //
	if(strchr(nd.pathmot[num],'/') != NULL)  WaitForKey = 1;
	if(strchr(nd.pathmot[num],'-') != NULL)  WaitForKey = 1;

	// Get Drive Letter //
	if(strcmp(nd.pathmot[num],"?") != 0)
	{
		strncpy(drve,nd.pathmot[num],2);
		drve[2] = '\0';
	}

	// Get Last \ to find position of Program Name //
	itshere = strrchr(nd.pathmot[num],'\\');
	strcpy(ProgramName,++itshere);


	// Lose the Drive Letter //
	strncpy(ProgramPath,nd.pathmot[num],(strlen(nd.pathmot[num])-strlen(ProgramName)-1));
	itshere2 = strpbrk(ProgramPath,"\\");
	strcpy(ProgramPath,++itshere2);
	ProgramPath[strlen(ProgramPath)] = '\0';

	if(strstr(ProgramName,".BAT"))
	{
		strcat(tmp,ProgramName);
		ProgramName[0] = '\0';
		strcpy(ProgramName,tmp);
	}

	strupr(ProgramName);

//==========
	handle = OpenFile(dosrunbat, O_WRONLY | O_CREAT | O_TRUNC, SH_DENYNO,S_IREAD | S_IWRITE);

//	handle = open("C:\\RUN\\DOSRUNN.BAT", O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
//	if(handle == -1)
//	{
//		cprintf("ERROR 127 -> DOSRUNN\n\r");
//		GetKey();
//	}
//==========

	if(strstr("",myargs))// if no args //
		sprintf(batbat,"@echo off\nCLS\n%s\nCD\\%s\n%s\nC:\nCD\\\RUN\nDOSRUN %ld %d %d\n",drve,ProgramPath,ProgramName,At_Page,At_Line,WaitForKey);
	else
	if(strstr(ProgramName,"?"))
	{
		ProgramName[(strlen(ProgramName)-1)] = ' ';
		sprintf(batbat,"@echo off\nCLS\n%s\nCD\\%s\n%s%s\nC:\nCD\\RUN\nDOSRUN %ld %d %d\n",drve,ProgramPath,ProgramName,myargs,At_Page,At_Line,WaitForKey);//space out %s%s//
	}
	else
		sprintf(batbat,"@echo off\nCLS\n%s\nCD\\%s\n%s%s\nC:\nCD\\RUN\nDOSRUN %ld %d %d\n",drve,ProgramPath,ProgramName,myargs,At_Page,At_Line,WaitForKey);//space out %s%s//
	write(handle,batbat,strlen(batbat));
//==========
	CloseFile(handle);
//	close(handle);
//==========
}

void DoMouse(void)
{
	InitMouse();
	if(Mouse_Detected == 1) 
	{
		MouseShape(0,0);// default shape //
		MouseOn();
		MoveMouse(1280,192);
		Rep(" Mouse Driver Installed ",0x7f,900);
	}
	else
	PostMessage("No Mouse Driver Installed",0);
}

static void interrupt newtimer(void)
{

	disable();

	tick++;

	enable();

	(*oldtimer)();

}

/*
static void interrupt newtimer(void)
{
	static int t;
	(*oldtimer)();

	t++;

	if(peekb(DosSeg,DosBusy) == 1) return;

	if(t >= 18) 
	{ 
		xt++; 
		ShowTimeTop(); 
		t = 0; 
	}

  if(xt == 60 && SW)
    StuffKey(CF3);
  else
		if(xt == 180)
			SaveScrn();
}
*/

void interrupt newkb(void)
{
	(*oldkb)();

	if(bioskey(1) == 15104)
	{
		GetKey(); 
		F1function();
	}
}

static void interrupt newctrlbrk(void)
{
	StuffKey(ESC);
	StuffKey(ALTQ);
	return;
}

static void interrupt newctrlc(void)
{
	return;
}
static void interrupt newpscreen(void)
{
	StuffKey(ESC);
	StuffKey(AF4);
	return;
}

void CheckForMenuKey(void)
{
	int kw = 0;
	int k  = 0;
	do
	{
		kw = peekb(0x0040,0x0017);
		if(kw & 8)// && k == 0)
		{	
			MenuBar(menubar);

			while(kw & 8 && k == 0)
			{
				kw = peekb(0x0040,0x0017);
				k = bioskey(1);
			}
      ShowDateTop();
		}
		CheckMouse();
		k = bioskey(1);
		Update();
	}while(k == 0);

	HelpNo = GlobalKey = GetKey();
  xt = 0;
	ShowRecords();
}

static int PostMessage(char *s, int test)
{
  int c;
	int strlength = (strlen(s)+14);
	int lf        = (40 - (strlength/2)-1);
	int rt        = lf+(strlength+4);
	if(test == 0) { lf = lf - 4; rt = rt + 4; }
	OpenWindow(lf,5,rt,9," REPORT ",0x4f4f,0x4f16);
	cprintf("\n\r ");
	cputs(s);
	cputs(test ? " (Y/ESC)..." : " Press any key... ");
	textcolor(WHITE);
	textbackground(LIGHTBLUE);
	do
	{
//		WalkMouse(364,504,48);
//		WalkMouse(504,364,48);
		c = bioskey(0) & 255;
	}while(test && tolower(c) != 'y' && tolower(c) != 'n' && c != ESC);
	CursorOff();
	CloseWindow();
	return(test && tolower(c) == 'y');
}

void Labels(void)
{
	WriteLabel(65,  4," ‘eN‘‘avigator...",0x4b,14);
	WriteLabel(65,  6," ‘eD‘‘os Commands",0x5b,14);
	WriteLabel(65,  8," ‘eL‘‘ist Menus  ",0x3b,14);
	WriteLabel(65, 10," ‘eF‘‘ind Program",0x2b,14);
	WriteLabel(65, 12," ‘eR‘‘un Program ",0x0b,14);
	WriteLabel(65, 14," ‘eP‘‘hone Book  ",0x1b,14);
	WriteLabel(65, 16," ‘eM‘‘essage Book",0x6b,14);
	WriteLabel(65, 18," ‘eT‘‘ime for ?  ",0x5b,14);

	WriteLabel(7, 22," ‘eD‘‘osrun: The Ultimate in DOS Menuing Systems.. (This is not a TSR)",0x1b,67);
}

void Start(void)
{
	int f;
	OpenProcess();

///==========
	f = OpenFile("C:\\RUN\\DOSRUN.CFG", O_RDWR | O_CREAT | O_BINARY,SH_DENYNO,S_IWRITE | S_IREAD );//create new

//	f = open("C:\\RUN\\DOSRUN.CFG", O_RDWR | O_CREAT | O_BINARY,S_IWRITE );//create new
//===========

	read( f,&fnd,sizeof fnd);

	OpenWindow(20,5,60,7," System Check ",0x1f1b,0x1f06);
	cprintf("\n\r Testing.....");
	GetC();
	delay(800);
	CloseWindow();

	if((fnd.s >> 12) < 8)//real colour//
		screencolour = fnd.s;

	if(fnd.vgachars == 1)
		system("VGAC MED");

	FillVideo(0,1,80,25,screencolour);//cyan

	strcat(TOPLINE2,fnd.reg);
	RunPrompt("",0x1e);

	if(fnd.dat == 0)
	{
		fnd.dat = mnth+256;
		write (f,&fnd,sizeof fnd);
  }

//==========
//	close(f);
	CloseFile(f);
//==========

	CalculateMenu(0, menu1); 
	CalculateMenu(1, menu2); 
	CalculateMenu(2, menu3); 
	CalculateMenu(3, menu4);
	CalculateMenu(4, menu5);
	CursorOff();
}

void End(void)
{
  if(Mouse_Detected == 1) 
    MouseOff();

	CursorOn();
	CloseAllWindows();
	if(fnd.vgachars == 1)
	{
//		cprintf("\n\rType: VGAC and press Enter to Deactivate VGAC Character Set\n\r");
		system("C:\\RUN\\VGAC");
		system("cls");
	}
	CloseProcess();
}

void HookVectors(void)
{
	#ifdef NODEBUG
	{
		disable();
		oldkb      = getvect(KYBRD);   setvect(KYBRD,  newkb);
		oldtimer   = getvect(TIMER1);  setvect(TIMER1, newtimer);
		oldctrlbrk = getvect(CTRLBRK); setvect(CTRLBRK,newctrlbrk);
		oldctrlc   = getvect(CTRLC);   setvect(CTRLC,  newctrlc);    
		oldpscreen = getvect(PSCRN);   setvect(PSCRN,  newpscreen);
		enable();
	}
	#endif
}

void UnhookVectors(void)
{
	#ifdef NODEBUG
	{
		disable();
		setvect(KYBRD,oldkb);
		setvect(TIMER1, oldtimer);
		setvect(CTRLBRK,oldctrlbrk);
		setvect(CTRLC,  oldctrlc);
		setvect(PSCRN,  oldpscreen);
		enable();
	}
	#endif
}

// error section //
error_win(char *msg)
{
	int retval;
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

		if (retval == 'i' || retval == 'I')
		{
			retval = IGNORE;
			break;
		}
	}
	return(retval);
}


/*
pragma warn -par reduces warnings which occur
due to the non use of the parameters errval,
bp and si to the handler.
*/

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

	 sprintf(msg, "Error: %s on drive %c\n\r A)bort, R)etry, I)gnore: ",
           err_msg[errorno], 'A' + drive);

	 OpenWindow(15,5,65,10,"[ Drive Error ] ",0x1f1f,0x1f0f);
   hardresume(error_win(msg));
	 CloseWindow();
   return ABORT;
}
#pragma warn +par

void WriteRegNo(void)
{
  UnhookVectors(); 
	WriteVideo(0,1,1,"",0x1f,80);
	WriteVideo(2,2,1,TOPLINE2,0x1f,0);
  GetKey();
  HookVectors(); 
  ShowDateTop();
}

int MenuMatches(void)
{
	// valid key ie keys < 256 are stuffed into keyboard //
	// all keys are returned //

	Window_Icon = 0;

	switch(Menu_Count)
	{
			case 0:
			switch(menuvars[Menu_Count].mline)
			{
				case 0:  return(F2);
				case 1:  return(F3);
				case 2:  return(F4);
				case 3:  return(F5);
				case 4:  return(F6);
				case 6:  return(F7);
				case 7:  return(F8);
				case 9:  return(F9);
				case 10: return(F10);
				case 11: return(ALTW);
				default: break;
			}
		case 1:
			switch(menuvars[Menu_Count].mline)
			{
				case 0:  return(AF2);
				case 1:  return(AF3);
				case 2:  return(AF4);
				case 3:  return(AF5);
				case 4:  return(AF6);
				case 6:  return(AF7);
				case 7:  return(AF8);
        case 9:  return(AF9);
        case 10: return(AF10);
				default: break;
			}
		case 2:
			switch(menuvars[Menu_Count].mline)
			{
				case 0:  return(CF1);
				case 1:  return(CF2);
				case 2:  return(CF3);
				case 3:  return(CF4);
				case 4:  return(CF5);
				case 6:  return(CF6);
				case 7:  return(CF7);
				case 8:  return(CF8);
				case 9:  return(CF9);
        case 10: return(CF10);
        case 12: return(ALTL);
        case 13: return(ALTR);
				default: break;
			}
		case 3:
			switch(menuvars[Menu_Count].mline)
			{
				case 0:  return('~');
				case 1:  return('?');
				case 2:  return('?');
				case 3:  return('?');
				case 5:  return('?');
				default: break;
			}
		case 4:
			switch(menuvars[Menu_Count].mline)
			{
				case 0:  Leave = 1;
                 return(ESC);
				case 2:  return(159);//ctrl + s//
				case 3:  return(177);//ctrl + s//
				default: break;
			}
		default: break;
	}
	return(0);
}

int MainLoop(RECORDENTRYFIELD *fld)
{
  char bra[61] = "[ ";
	char c;
  int ii = 0;
  int tl = 0;

	static int templock;
//	static int lx;        // used for Zips //

  RECORDENTRYFIELD *firstfld,*lastfld, *begining;

  begining = firstfld = lastfld = fld;

	while(lastfld->fcol)
		lastfld++;

  lastfld--; 
  firstfld++;
  firstfld++;
  fld = firstfld;

	for(i = 2; i < At_Line; i++) 
		fld++;

  WriteVideo(1,(fld->fcol+XOFF-1),(fld->frow+YOFF)," ",0x4f,Plength+2);
	WriteVideo(1,(fld->fcol+XOFF),(fld->frow+YOFF),(fld->fbuff),0x4f,Plength);

	if(strnicmp("",nd.mpmot,1))
		WriteVideo(1,55,1,"[í]",0x1e,0);
	else
		WriteVideo(1,55,1,"ÍÍÍ",0x1f+0x01,0);

	Leave = 0;

	while(Leave != 1)
	{
    ShowRecords();

    if(Record_Count == 0)
			StuffKey(AF2);

    LinePosition = fld->frow+YOFF-1;
		gotoxy(1,LinePosition);
    ShowPath(LinePosition);

    Busy_Flag = 0;

		if(Message_Waiting != 0)
		{
			ClearBiosBuff();
			StuffKey('T');//ALTT
		}

    HelpNo = F1;
		CheckForMenuKey();

    LinePosition = fld->frow+YOFF-1;
    gotoxy(1,LinePosition);
    ShowPath(LinePosition);

		CursorOff();
    WriteVideo(1,(fld->fcol+XOFF-1),(fld->frow+YOFF)," ",0x1f,Plength+2);
		WriteVideo(1,(fld->fcol+XOFF),(fld->frow+YOFF),(fld->fbuff),(fld->color),Plength);

		switch(GlobalKey)
		{
			//MENUKEYS//
			case 255:  MatchMouse();	break;
			case ALTP:  
			case ALTM:
			case ALTU:
			case ALTQ:
			case ALTZ:
								INMENU = 1;
								GlobalKey = ProcessMenuKeys(GlobalKey);
								Launch_Flag = 0;
								INMENU = 0;
                ShowDateTop();
								break;

			case 'p':
			case 'P':
								OpenWindow(20,5,60,7," System Check ",0x1f1b,0x1f06);
								cprintf(Launching);
                delay(500);
								CloseWindow();
			          UnhookVectors();
								spawnl(P_WAIT, "C:\\RUN\\PBOOK.EXE"  , NULL);
								HookVectors();
								CursorOff();
								break;

      case 'm':
      case 'M': 
								OpenWindow(20,5,60,7," System Check ",0x1f1b,0x1f06);
								cprintf(Launching);
                delay(500);
                CloseWindow();
			          UnhookVectors();
								spawnl(P_WAIT, "C:\\RUN\\MBOOK.EXE"  , NULL);
								HookVectors();
								CursorOff();
								break;
			case ALTW:
			case 'W':
			case 'w':  WindowsOptions(LinePosition);
								 if(GlobalKey == 'Y' || GlobalKey == 'y')
								 {
									 SeekBeginingCurRcd(fd1);
									 write(fd1, &nd,sizeof nd);
									 Current_Record = tell(fd1);
									 WriteScrnRecord(Per_Entry_Field);
									 GlobalKey = 0;
                 }
			           break;

			case TAB: break;

			case CTRLHOME:   FirstRecord(); break;

			case CTRLEND:    LastRecord();  break;

			case UP:
								if(fld > firstfld) fld--; else fld = lastfld;  break;
		
			case DOWN:
								if(fld < lastfld)  fld++; else fld = firstfld; break;

			case HOME:
								 fld = firstfld; break;

			case END:  fld = lastfld;  break;

			case LEFT:                                           // left
			case PGUP: 
									if(Record_Count < 2) 
                  	break;
								  PrevRecord(); 
									break;

			case RIGHT:                                          // right
			case PGDN:  
									if(Record_Count < 2)
                  	break;
									NextRecord();  
									break;

			case F1:   //Rep("Programme Options. Move the cursor to an option and press F1 for help",0x4f,1);
								 break;
			case AF1:  //Rep("Menu Options. Move the cursor to an option and press F1 for help",0x4f,1);
								 break;

			case F2:   fld = firstfld;
                 while(strcmp("",fld->fbuff) != 0)
								 { ++fld; LinePosition = fld->frow+YOFF-1; } // go to blank ?
                 if(LinePosition < 11)
                 {
                   StuffKey(INS);
                 }
                 else
                 {
                   StuffKey(AF2);
									 fld = firstfld;
                   LinePosition = fld->frow+YOFF-1;
                 }
                 break;

		case AF2:	  	LastRecord();
                	WriteVideo(1,55,1,"ÍÍÍ",0x1f+0x01,0); 
                	CursorOn();
									Rep("Creating New Menu",4,10);
                	BlankARecord(Per_Entry_Field);
		           		OpenWindow(10,5,70,8," Enter Menu Name ",0x3f3f,0x3f0f);
		           		gotoxy(5,2);

                	strset(begining->fbuff,'\0');

		           		i = EditLine(begining->fbuff,Legal_Text,fld->flen-1,0x3f,Edit,0);
                	CloseWindow();
                	CursorOff();

									strcat(begining->fbuff," ]");
									strcat(bra,begining->fbuff),
                	strcpy(begining->fbuff,bra);
                	strset(bra,'\0');
                	strcpy(bra,"[ ");

//                	WriteTopOfMenu(Per_Entry_Field);
                	Rep("",4,0);

                	if(i != ESC)
                	{
										memset(nd.mpmot,'\0',21);
                  	for(i = 0; i < 11; i++)
                  	strset(nd.pathmot[i],'\0');

                  	SaveRecord();
                	}
                	else
                		ThisRecord();
                	break;

			case F4:
			case INS:  	EditALine(fld);
                	break;


      case AF4:  
                	WriteVideo(1,55,1,"ÍÍÍ",0x1f+0x01,0); 
									Rep("Change Menu Name",0xf,10);
                	CursorOn();
		           		OpenWindow(10,5,70,8," Enter Menu Name ",0x3f3f,0x3f0f);
		           		gotoxy(5,2);
                	i = EditLine(begining->fbuff,Legal_Text,fld->flen,0x3f,1,0);
                	CloseWindow();
                	CursorOff();
//                	WriteTopOfMenu(Per_Entry_Field);

                	if(i != ESC)
                	{
										SeekBeginingCurRcd(fd1);
		            		write(fd1, &nd,sizeof nd);
										Current_Record = tell(fd1);
		            		WriteScrnRecord(Per_Entry_Field);
                 	}
                 	else
                  	ThisRecord();
                 	Rep("",0xf,0);
                 	break;

			case DEL:   
			case 8:  
			case F3:    
									if(strnicmp("",nd.mpmot,1))
				         	{
									 Rep(menuprotect,0xcf,1);
                	}
                	else

                	if(YesNo(" Delete this Item ? "))
									{
		         		  	strnset(nd.pathmot[LinePosition-1],'\0',ZPL);
										strnset(fld->fbuff,'\0',fld->flen);
										SeekBeginingCurRcd(fd1);
                  	write(fd1, &nd,sizeof nd);
                  	Current_Record = tell(fd1);
                  	WriteScrnRecord(Per_Entry_Field);
                	}
                 	break;			

			case AF3:		if(YesNo(" Delete this Menu ? "))
									{ 
	             			if(Record_Count == 1)
                   	{
                   		chsize(fd1,0); //DeleteFile(dosrunrec);
                     	BlankARecord(Per_Entry_Field);
                      ShowRecords();
                      break;
                   	}
                   	else
											DeleteRecord(); 
									 	}
									break;

			case CF5:  MouseOff(); break;

			case F5:   
			case AF5:  TextSearch();  
                 if(Found_Flag == 1)
                 {
                   LinePosition = At_Line;
                   fld = firstfld; // reset to first //
                   for(i = 1; i < At_Line; i++) fld++; // calculate new line
                 }
                 break;

      case 'l':
      case 'L':
			case AF6:  ListMenus(); break;

			case F6:	 if(strnicmp("",nd.mpmot,1))
				         {
									 Beep(100,500);
					         Rep(menuprotect,0x4f,1);
					         break;
								 }
								 SaveToClip(Per_Entry_Field,wherey());
									break;

      case AF7:
			case F7:  if(strnicmp("",nd.mpmot,1))
								 {
									 if(!PwProt())
										break;
								 }
								 OpenWindow(21,6,61,8," Protection ",0x3f3f,0x3f06);
								 cprintf("\n\r    Enter New Password: [          ]");
								 cprintf("\r    Enter New Password: [");
								 c = ' ';
								 memset(nd.mpmot,' ',21);
			           ii = 0;
			           while(c != CR && c != ESC) 
								 {
				           c = GetKey();
				           cprintf("*");
				           if(c != ESC)
									 nd.mpmot[ii] = c + 128 + ii + c;

				           if(ii == 10 && c != CR)
				           {
					           Buzzer();
                     c = GetKey();
				           }
                   ii++;
			           }
			           CloseWindow();

			           if(c == CR)
			           {
									 nd.mpmot[--ii] = '\0';
									 SeekBeginingCurRcd(fd1);
		               write(fd1, &nd,sizeof nd);
									 Current_Record = tell(fd1);
		               WriteScrnRecord(Per_Entry_Field);
			           }
                 else
								 strset(nd.mpmot,'\0');

                 break;

			case F8:
      case AF8: 
      case AF9: break;

      case F9:   
     	case F10:  
								OpenWindow(3,8,77,16,"[ Navigator (DosRun) Automatic File Find Utility ]",0x0f0f,0x0f06);
                CursorOn();
								GetPath(LinePosition);
                CloseWindow();
                CursorOff();
                ThisRecord();
								break;

			case ALTL:	
								WriteRegNo(); 
								break;
			case ALTR:
								UnhookVectors();
								spawnl(P_WAIT, "C:\\RUN\\READ.EXE","C:\\RUN\\READ.EXE","C:\\RUN\\DOSRUN.TXT",NULL);
								//ReadFile("C:\\RUN\\DOSRUN.TXT",3,4,78,19);
								HookVectors();
								CursorOff();
								break;

			case CF10: OpenWindow(20,7,57,10," Tempory Lock Enabled ",0x4f4f,0x4f04);
								 cprintf("\n\r    Enter Key:");
								 ClearBiosBuff();
								 tl = GetKey();
								 clrscr();
								 cprintf("\n\r    System Tempory Lock Active");

								 templock = 0;
								 do
								 {
									 if(templock == 3)
									 {
										 Buzzer(); clrscr(); textcolor(WHITE+BLINK);

                 		 cprintf("\n\r      System Lock-Out Active");
										 delay(5000);

										 ClearBiosBuff(); clrscr(); textcolor(WHITE);

										 cprintf("\n\r      System Lock-Out Active");
										 templock = 0;
									 }
									 templock++;
								 }while(GetKey() != tl);

								 CloseWindow();
								 break;
			case CF1:  Mot(0); break;
      case CF2:  CallNoPrint(); break;
			case CF3:  MikeInfo();  break;

      case CF4:  if(Mouse_Detected == 0) 
                   DoMouse(); break;

			case CF6:  OpenWindow(1,1,80,25,"",0x1f1f,0x1f00);
			           CloseProcess();
								 cprintf("Press any Key");
			           GetKey();
                 cprintf("\n\r");
                 textcolor(LIGHTGRAY);
			           OpenProcess();
								 CloseWindow();

								 break;
			case CF7:  MenuSummary();break;
			case CF8:  UnhookVectors();
								 spawnl(P_WAIT, "C:\\RUN\\DOSRUNU.EXE"  , NULL);
								 HookVectors();
								 CursorOff();
								 break;

			case CF9:  At_Page = tell(fd1); At_Line = LinePosition; Launch_Flag = 1;
								 strcpy(nd.pathmot[LinePosition-1],"C:\\RUN\\DOSRUNC.EXE");
								 return(LinePosition);

			case ESC:  if(Buffer_Full > 0)
								 {
										Rep("You have not completed a file move.. Move or Delete it!",0x0e+BLINK,10);
										Beep(400,200);
										Beep(600,300);
										Beep(400,200);
										delay(2000);
										Leave = 0;
										break;
								 }
								 //else
								 //if(YesNo(" Exit Programme ? "))
								 //{
										 Leave = 1;
								 //}
								 //else
								 //	 Leave = 0;

								 if(Leave == 1)
								 {
									 if(strlen(mott) > 3)
										 if(!Mot(Leave))
										 {
											 Leave = 0;
											 break;
										 }
									 Window_Icon = 0x8403;
									 OpenWindow(15,7,65,11," Bye & Thanks ",0x0f0f,0x0409);
									 WriteVideo(3,1,3," DosRun Version 7.01 System Log Off. ",0x0f,0);
									 delay(1000);
									 CloseWindow();
									 return(0);
								 }
								 break;
			case CR:
				if(!strnicmp("",nd.pathmot[LinePosition-1],1)) // IF BLANK BREAK
				{
					Launch_Flag = 0;
					break;
				}
				else
				if(strnicmp("",nd.mpmot,1))
				{
					if(!PwProt())
					break;
				}
				//strcpy(ThisLine,fld->fbuff);

if( strstr(fld->fbuff,"mp3") || strstr(fld->fbuff,"MP3"))
{
	strcpy(myargs," ");
	strcat(myargs,fld->fbuff);
}

				DosArg(LinePosition);

				if(GlobalKey == ESC)
					break;

				At_Page = tell(fd1); At_Line = LinePosition; Launch_Flag = 1;
				return(LinePosition);

			//ctrl + s //
			case 159:  if(strlen(mott) > 3)
								 if(!Mot(1))
								 {
									 Leave = 0;
									 break;
								 }
								 At_Page = tell(fd1); At_Line = LinePosition; Launch_Flag = 1;
								 strcpy(nd.pathmot[LinePosition-1],"C:\\COMMAND");
                 return(LinePosition); 

      case 'r':
      case 'R': OpenWindow(3,21,77,23," Run Programme ",0x0f0f,0x0b05);
                 WriteVideo(3,1,2,"Not Available in this Version",0x0f,0);
                 GetKey();
                 CloseWindow();
                 break;

      case 'S':
      case 's':
			case 'F':
			case 'f': StuffKey(AF5); break;

//			case CF10: 

      case 'n':
      case 'N':
      case 't':
      case 'T':
      case 'd':
			case 'D': LabelWindows();
								if(GlobalCall != 0)
									StuffKey(GlobalCall);

								 if(GlobalKey == CR)//for dos helper//
                 {
                   strcpy(nd.pathmot[LinePosition-1],"?");
                   At_Page = tell(fd1); At_Line = LinePosition; Launch_Flag = 1;
                   return(LinePosition);
                 }
                 else
                 break;
			case '~':
								SubMenu(60, 3,"",Zips,NULL,0x0101,0x010f);
								break;

			case '?':
						 Rep(" *** Not Available in this Version *** ",0x4f,1);
						 Rep("  ",0x1f,0);
						 break;

			case 177:  ShowLockedFiles(); break;

			default:   RunPrompt("",0x1f);
								 if(GlobalKey == 0)
									 break;
                 if(GlobalKey > 126 && GlobalKey < 248) 
								 break;
		}
		if(strnicmp("",nd.mpmot,1))
	  WriteVideo(1,55,1,"[í]",0x1e,0);
	  else
	  WriteVideo(1,55,1,"ÍÍÍ",0x1f+0x01,0);

    ShowPath(LinePosition);
		CursorOff();
    WriteVideo(1,(fld->fcol+XOFF-1),(fld->frow+YOFF)," ",0x4f,Plength+2);
    WriteVideo(1,(fld->fcol+XOFF),(fld->frow+YOFF),(fld->fbuff),0x4f,Plength);    

		MouseOff();
		MouseOn();
	}
	return(GlobalKey);
}

void F1function(void)
{
  if(HelpNo == F1)
  {
    StuffKey(CF7);
    return;
  }
  RunItNow(100);
}

void ShowTimeTop(void)
{
	union REGS regs;
	regs.h.ah = 0x2c;
	intdos(&regs,&regs);
	sprintf(timeis,"%02.2d:%02.2d:%02.2d",regs.h.ch,regs.h.cl,regs.h.dh);

	WriteVideo(0,72,1,timeis,0x10+0x0b,9); 
	UpDateMessage();
}

void ShowDateTop(void)
{
	union REGS regs;
	regs.h.ah = 0x2a;
	intdos(&regs,&regs);
  mnth = regs.h.dh;
	sprintf(dateis,"%02.2d/%02.2d/%02.2d",regs.h.dl,regs.h.dh,regs.x.cx-2000);

	WriteVideo(0,1,1," ",0x0b+0x10,10);
	WriteVideo(0,2,1,dateis,0x0b+0x10,9);
	WriteVideo(2,2,1,TOPLINE,0x1f,0);
}

static int EditALine(RECORDENTRYFIELD *fld)
{

	Rep(enterdata,0x4f,10);

	if(strcmp(fld->fbuff,""))
		Edit = 1;
	else
		Edit = 0;

	WriteVideo(1,(fld->fcol)+XOFF,(fld->frow+YOFF),"",0x3f,Plength);

	gotoxy(fld->fcol+XOFF-1, fld->frow+YOFF-1);//positioning//
	CursorOn();
	GlobalKey = EditLine(fld->fbuff,Legal_Text,fld->flen-1,0x3f,Edit,0);
	CursorOff();

	if(GlobalKey == ESC) //put back old line //
		ThisRecord();
	else
	if(GlobalKey == CR)// || GlobalKey == F2)
	{
		SeekBeginingCurRcd(fd1);
		write(fd1, &nd,sizeof nd);
		Current_Record = tell(fd1);
		WriteScrnRecord(Per_Entry_Field);
	}
	Rep("",4,0);
	if(GlobalKey == CR && Edit == 0)
		StuffKey(F10);
	return(GlobalKey);
}

static void SaveRecord(void)
{
	if(GlobalKey == AF2)
	{
		Beep(100,10);
		write(fd1, &nd,sizeof nd);
		Current_Record = tell(fd1);
		WriteScrnRecord(Per_Entry_Field);
	}
	else

	if(GlobalKey == ESC)
	{
		lseek(fd1, Current_Record, SEEK_SET);
		ThisRecord();
    WriteScrnRecord(Per_Entry_Field);
	}
}

static void TextSearch(void)
{
  int i = 0;

	searchitem[0] = '\0';
	BlankARecord(Per_Entry_Field);

	OpenWindow(15,8,65,11," Search for a Programme or Menu ",0x3f3f,0x3f1f);
	cprintf("\n\r Search For ? ");

	GlobalKey = 0;

	while(GlobalKey != ESC && GlobalKey != '\r')
	{
		gotoxy(15,3);
		CursorOn();
		GlobalKey = EditLine(searchfor,Legal_Text,35,0x3f,0,0);
    ToLower(searchfor);
	}

	if(GlobalKey != '\r')
	{
		CloseWindow();
		Current_Record = tell(fd1);
		ThisRecord();
		return;
	}

  lseek(fd1, 0L, SEEK_SET);
	
	while(TRUE)
	{
		read(fd1, &nd, sizeof nd);

    for(i = 1; i < 13; i++)
    {
      switch(i)
      {
				case 1:  strcpy(searchitem,nd.menuname); break;
				case 2:  strcpy(searchitem,nd.item1);  break;
				case 3:  strcpy(searchitem,nd.item2);  break;
				case 4:  strcpy(searchitem,nd.item3);  break;
				case 5:  strcpy(searchitem,nd.item4);  break;
				case 6:  strcpy(searchitem,nd.item5);  break;
				case 7:  strcpy(searchitem,nd.item6);  break;
				case 8:	 strcpy(searchitem,nd.item7);  break;
				case 9:  strcpy(searchitem,nd.item8);  break;
				case 10: strcpy(searchitem,nd.item9);  break;
				case 11: strcpy(searchitem,nd.item10);  break;
			}
			ToLower(searchitem);
			if(strstr(searchitem,searchfor) != NULL)   // look for menu first //
			{
				Current_Record = tell(fd1);
				At_Line = i-1;
				Found_Flag = 1;
				CloseWindow();
				Current_Record = tell(fd1);
				ThisRecord();
				return;
			}
		}
    if(eof(fd1))
			break;
	}
	cprintf(" \n\r\n\r Item Not Found");
	delay(1000);
	Current_Record = tell(fd1);

	CloseWindow();
	ThisRecord();
	GlobalKey = 0;
}

static void FirstRecord(void)
{
	lseek(fd1, 0L, SEEK_SET);
	NextRecord();
}

static void NextRecord(void)
{
	if(eof(fd1))
		lseek(fd1, 0L, SEEK_SET);          // if end ? go begining
	BlankARecord(Per_Entry_Field);
	read(fd1, &nd, sizeof nd);           // read in a group of data
	Current_Record = tell(fd1);          // get current position
	WriteScrnRecord(Per_Entry_Field);
}

static void LastRecord(void)
{
	lseek(fd1, 0L, SEEK_END);
	ThisRecord();
}

static void PrevRecord(void)
{
	SeekBeginingCurRcd(fd1);
	if(tell(fd1) == 0L)
	lseek(fd1, 0L, SEEK_END);
	ThisRecord();
}

static void SeekBeginingCurRcd(int f) //seek_this_record
{
	if(f == fd1)
	{
		if(tell(fd1) != 0L)
			lseek(fd1, -((long) sizeof nd), SEEK_CUR);
	}
	else
	{
		if(tell(fd2) != 0L)
			lseek(fd2, -((long) sizeof tnd), SEEK_CUR);
	}
}

static void ThisRecord(void)
{
		SeekBeginingCurRcd(fd1);
		NextRecord();
}

static void DeleteRecord(void)
{                                                
	int end = FALSE;                               // set no finished
	long here = tell(fd1);                         // mark here

	if(here == 0) return;                          // if at end return
																								 
	while(end == FALSE)                            // loop till end
	{                                             
		if((end = eof(fd1)) == FALSE)                // check end again
		{
			read(fd1, &nd, sizeof nd);
			end = eof(fd1);	                           // if end = 1 else 0
			lseek(fd1,-((long)sizeof nd*2), SEEK_CUR);  // goback 2 records
			write(fd1, &nd, sizeof nd);                //
		}
		else
		lseek(fd1,-((long)sizeof nd), SEEK_CUR);

		if(end) 
			chsize(fd1,(long)tell(fd1));
		else
			lseek(fd1,(long)sizeof nd,SEEK_CUR);
	}

	lseek(fd1, here, SEEK_SET);
	BlankARecord(Per_Entry_Field);
	ThisRecord();
}

static void BlankARecord(RECORDENTRYFIELD *fld)
{
	int i;
  memset(nd.menuname,'\0',ZNL);
	WriteVideo(1,2,1,doubleline,0x1f+0x01,0); //delay(300);
	for(i = 2; (fld+i)->fcol; i++)
	{
		WriteVideo(1,(fld+i)->fcol+XOFF,(fld+i)->frow+YOFF,"",(fld+i)->color,(fld+i)->flen);
		setmem((fld+i)->fbuff,(fld+i)->flen,'\0');
	}
	WriteVideo(1,8,15,"=]",0x1f,0);
	WriteVideo(1,10,15,"ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ",0x1f+0x01,0);
}


void CallCritError(int i)
{
	if(i == 0)
	{
		Rep("Error: 0 Note this Number for Technical Help.... Press any Key ",0x1f,1);
		return;
  }
	UnhookVectors();
	End();
	cprintf("Error [%d]",i);
	exit(0);
}

void WriteScrnRecord(RECORDENTRYFIELD *fld)
{
//	static nl;
	int i = 0;
//	if(strlen(nd.menuname) < nl)
//		WriteVideo(1,2,1,doubleline,0x1f+0x01,0);

	WriteVideo(3,1,1,(fld->fbuff),0x1f,0);
//	WriteTopOfMenu(Per_Entry_Field);

	for(i = 2; (fld+i)->color; i++)
		WriteVideo(1,((fld+i)->fcol)+XOFF,((fld+i)->frow+YOFF),((fld+i)->fbuff),((fld+i)->color),0);
//		nl = strlen(nd.menuname);
}

void SetKeys(void)
{
 	_AH = 0x34;
	geninterrupt(0x21);
	DosSeg =  _ES;
	DosBusy = _BX;
	pokeb(0x0000,0x0417,32);
}

static void ToLower(char *s)
{
	while(*s)
	{
		*s = tolower(*s); 
		s++;
	}
}


void CallNoPrint(void)
{
	 OpenWindow(10,5,70,10," Printing ",0x5f5f,0x5f1f);
	 cprintf("\n\r    The Dos Navigator Programme has no Printable Files.. \n\r");
	 while(GetKey() != ESC);
	 CloseWindow();
}

void MikeInfo(void)
{
	OpenWindow(3,3,78,22,"  (C) Michael O'Toole 1984-1996 To Register Contact ",0x1c1a,0x1a00);
	CursorOff();
	textcolor(WHITE);
	gotoxy(29,4);
	cprintf(" Michael O'Toole");
	gotoxy(28,5);
	cprintf(" d");
	gotoxy(31,6);
	cprintf(" ");
	gotoxy(32,7);
	cprintf(" ");
	gotoxy(33,8);
	cprintf(" Ireland");
	gotoxy(27,9);
	cprintf("Email: phpbbirelang@gmail.com");

	gotoxy(26,11);
	cprintf(" Phone: 353-01-6234006");
	gotoxy(22,14);
	cprintf("The Lighthouse Software Company (R) \n\r");
	gotoxy(17,17);
	cprintf("œ20 for private use & œ50.00 for Company Use");
	if(SW)
	{
	  textcolor(YELLOW+BLINK);
	  cprintf("\n\r\n\r                             PLEASE REGISTER!           \n\r");
	  textcolor(WHITE);
	}
  GetKey();
	CloseWindow();
}


int YesNo(char *ques)
{
	return PostMessage(ques,TRUE);
}

void WriteLabel(int sx, int sy, char *line, int nat, int tok)
{
	int ok = 0;
	int shaddow = 0;//0x79;

	i = (screencolour >> 12);
		shaddow = i;

	if((screencolour <<  8) == 0)  //plane colour//
	{
		shaddow = (shaddow << 4) + 0x09;
		if((nat >> 4) == i)
			nat = 0x2b;
       ok = 1;
	}

	WriteVideo(0,sx,sy,line,nat,tok);

   if(ok)
   {
	  WriteVideo(0,sx+tok,sy ,"Ü",shaddow,0); 

	  for(i = sx+1; i < sx + tok + 1; i++)
     {
		  WriteVideo(0,i,sy+1,"ß",shaddow,0);
     }
   }
}

void SaveToClip(RECORDENTRYFIELD *fld, int ms)
{
	int i;

	for(i = 1; i <= ms; i++) 
	fld++;

	if(strcmp(fld->fbuff,"") == 0 && Buffer_Full == 0)
  {	
    GlobalKey = 0;
    return;
  }

	if(Buffer_Full == 0)
	{
		Rep(movebuffer,0x4+BLINK,10);
		strcpy(ClipLines.ClipItemA,fld->fbuff);
		strcpy(ClipLines.ClipPathmotA,nd.pathmot[ms-1]);
		memset(fld->fbuff,'\0',ZDL);
		memset(nd.pathmot[ms-1],'\0',ZPL);
		Buffer_Full = 1;
	}
	else
	if(Buffer_Full == 1 || Buffer_Full == 2)
	{
		if(strcmp(fld->fbuff,"") == 0)
		{
			strcpy(fld->fbuff,ClipLines.ClipItemA);
			strcpy(nd.pathmot[ms-1],ClipLines.ClipPathmotA);
			Rep("",4,0);
			Buffer_Full = 0;
		}
		else
		{
			Rep("Exchange Line ? Previous Line Stored In Clip Board",0x4+BLINK,10);
			strcpy(ClipLines.ClipItemB,fld->fbuff);
			strcpy(ClipLines.ClipPathmotB,nd.pathmot[ms-1]);
			memset(fld->fbuff,'\0',ZDL);
			memset(nd.pathmot[ms-1],'\0',ZPL);
			strcpy(fld->fbuff,ClipLines.ClipItemA);
			strcpy(nd.pathmot[ms-1],ClipLines.ClipPathmotA);
			// swap //
			strcpy(ClipLines.ClipItemA,ClipLines.ClipItemB);
			strcpy(ClipLines.ClipPathmotA,ClipLines.ClipPathmotB);
			Buffer_Full = 2;
		}
	}
	SeekBeginingCurRcd(fd1);
	write(fd1, &nd,sizeof nd);
	Current_Record = tell(fd1);
}

int PwProt(void)
{
	int i = 0;
	char c = ' ';
	char wq[40];

	OpenWindow(20,6,60,9,"[ This Menu has Password Protection ]",0x4b4b,0x4b06);
	cprintf("\n\r     Enter Password: [          ]\r");
	cprintf("     Enter Password: [");
	while(c != CR && c != ESC) 
	{
		c = GetKey();
		if(c != ESC)
		wq[i] = c + 128 + i + c;
		cprintf("*");
		if(i == 10 && c != CR)
		{
			Buzzer();
		}
    i++;
	}
	wq[--i] = '\0';

//	cprintf("[%s]",wq); GetKey();

	CloseWindow();
	return(!strcmp(nd.mpmot,wq));
}

int Mot(int ponly)
{
	char mpw[11];
	char c = ' ',cc;
	char lop[11];
	int i = 0;
	int set = 0;

  if(ponly == 1)
  	OpenWindow(15,6,65,10,"[ EXIT ? ENTER MASTER PASSWORD ]",0x0b0b,0x0b06);
  else
	OpenWindow(15,6,65,10,"[ MASTER PASSWORD - REVEAL CODES ]",0x0b0b,0x0b06);
	textcolor(LIGHTCYAN);
	cprintf("\n\r   Enter Master Password: ");
	textcolor(YELLOW);
	
	while(c != CR || c != CR)
	{
		c = GetKey();
		mpw[i++] = c;
		cprintf("*");
	}

	mpw[--i] = '\0';
	i = 0;
	c = ' ';

	if(strcmp(mpw,mott))
	{	
		CloseWindow();
		return(0);
	}
	clrscr();
	cprintf(" ");

	while(i < strlen(nd.mpmot)) 
	{
		c = nd.mpmot[i];

		if(c < 1)
			c = c + 256;

		cc =  (c - 128) - i;
		cc = cc / 2 ; 

		if(cc < 0)
			cc = cc + 128;

		lop[i++] = cc;
		if(cc == 0)
			set = 1;
	}
	lop[i] = '\0';

	textcolor(LIGHTCYAN);
	if(set == 1)
	{
		cprintf("\n\r Second Level Protection- Can't Reveal"); 
		GetKey();
	}
	else
	{
    if(ponly == 0) // dont show awmot //
    {
		  cprintf("\n\r    The Password for this Menu:");
      textcolor(WHITE);
      cprintf(" %s",lop);
		  GetKey();
      textcolor(CYAN);
    }
	}
	CloseWindow();
  return(1);
}

void UpDateMessage(void)
{
  int ii;

  if(Message_Waiting == 0 && Busy_Flag == 0)
  {
    for(ii = 3;  ii <= 20; ii++)
    {
      if(strncmpi("N",(trec+ii)->fbuff,1) == 0) 
      {
        ii = ii - 3;
        if(strncmpi(dateis,(trec+ii)->fbuff,8) == 0)
        {
          ii++;
          if(strncmpi(timeis,(trec+ii)->fbuff,5) >= 0 && strncmpi(" " ,(trec+ii)->fbuff,1) <= 0) 
          {
            RunPrompt("A Message is Waiting....",0x0c+BLINK);
            Message_Waiting = ii + 2;//go to Y/N
						return;
          }else ii = ii + 5;
        }else ii = ii + 6;
      }else ii = ii + 3;
    }
  }
}

void Rep(char *i,int color, int wait)
{
  // 0 = clear; 1 = display no clear; >100 = display for wait time;

	switch(wait)
	{
		case 0:  FillVideo(0,23,80,24,screencolour); return;
		case 1:  WriteVideo(0,1,24," ",color,80);
						 WriteVideo(2,0,24,i,color,0);
						 GetKey();
						 FillVideo(0,23,80,24,screencolour);	
						 break;
		case 10: WriteVideo(0,1,24," ",color,80);
						 WriteVideo(2,0,24,i,color,0);
						 break;
		default: WriteVideo(2,0,24,i,color,0);
						 delay(wait); 
						 FillVideo(0,23,80,24,screencolour);
						 break;
	}
}

static void GetPath(int num)
{
	char *ptr;
	char name[13];
	char drv1 = ' ';
	char s[ZPL];
	char temp[5] = "    ";
	strset(temp,'\0');

	if(GlobalKey == F9)
	{
		while(GlobalKey != CR && GlobalKey != ESC)
		{
			strcpy(s,nd.pathmot[num-1]);        // save bubber
			memset(nd.pathmot[num-1],'\0',61);  // clean this buffer
			strcpy(nd.pathmot[num-1],s);        // restore buffer
			WriteVideo(3,2,3,"To pass arguments / switches to this programme just add a ",0x0b,0);
			WriteVideo(3,2,4,"‘fspace‘‘ and a ‘f?‘‘ at the end. Example: ‘fC:\\DOS\\SCAN ?       ",0x0b,0);
			gotoxy(3,6);
			GlobalKey = EditLine(nd.pathmot[num-1],Legal_Text,60,0x0f,1,0); // F11
			strcpy(nd.pathmot[num-1],(strupr(nd.pathmot[num-1])));
		}
	}
	else
	if(GlobalKey == F10)
	{
		Rep("You can modify the path at a later stage using F9",0xf,10);
    clrscr();
		WriteVideo(3,1,3,"  Enter the Programme Name: eg TURBOC.EXE  or just TUR  ",0x1e,0);
		WriteVideo(1,4,5," Name: ",0x1f,68);
		gotoxy(10,4);
		memset(nd.pathmot[num-1],'\0',61);
    CursorOn();
		GlobalKey = EditLine(nd.pathmot[num-1],Legal_Text,55,0x1f,1,0);
    CursorOff();
    if(strlen(nd.pathmot[num-1]) > 0) 
		if(GlobalKey == CR)
		{
			strupr(nd.pathmot[num-1]);
			if(FindIt(drv1,nd.pathmot[num-1]))
			{
				strcpy(nd.pathmot[num-1],The_Path);

				// check to see if you are calling win.com, if so do nothing//
				if(strstr(nd.pathmot[num-1],"WIN.COM") != NULL);
				else
				if(strstr(nd.pathmot[num-1],"WIN") != NULL)
				{
					gotoxy(20,7);
					cprintf(" Is this a Windows Programme ? Y/N" );
					i = GetKey();
					if(i == 'Y' || i == 'y')
					{
						ptr = strrchr(nd.pathmot[num-1],'\\');   // find last /
						strcpy(name,++ptr);
						--ptr;

						strcpy(ptr,'\0');
						strcat(nd.pathmot[num-1],"\\ WIN ");
						strcat(nd.pathmot[num-1],name);
					}
				}
			}
			else
			strset(nd.pathmot[num-1],'\0');
		}
	}
  Rep(" ",0,0);
	if(GlobalKey == CR)
	{
		Beep(500,100);
		SeekBeginingCurRcd(fd1);
		write(fd1, &nd,sizeof nd);
		Current_Record = tell(fd1);
		return;
	}               
	else
	strset(temp,'\0');
	return;
}

int FindIt(char dr, char *buf)
{
	int matchfound = 0;
	int kout = 0;
	char dosrundat[30] = "C:\\RUN\\DOSRUN.DAT";
	char tmp[71];

	FILE *datafile;
	strset(The_Path,'\0');

	strcpy(tmp,buf);

	toupper(dr);
//	sprintf(dosrundat,"C:\\DOS\\DOSDAT.DAT"); //

	if((datafile = fopen(dosrundat,"r")) == NULL)
		{
			cprintf("ERROR OPENING %s ",dosrundat);
			GetKey();
			return(-1);
		}
	fseek(datafile, 0L, SEEK_SET);

	while(fgets(The_Path,ZPL,datafile) && kout != CR)
	{
		if(strstr(The_Path,tmp) != NULL)
		{
			matchfound = 1;
			strcpy(tmp,The_Path);
			WriteVideo(1,5,3," ",0x00,68);//clear
			WriteVideo(3,1,3,"[ Match Found! ]",0x4f,0);
			WriteVideo(3,1,5,"     Press Enter to accept, or any other key to continue search     ",0x0f,0);

			WriteVideo(1,3,7," ",0x1f,71);
			gotoxy(3,6);
			if(strlen(The_Path) >= 70)
			{
				cprintf("Path Length exceeds [70]... Option ? Create a Bath File to call it ?");
				Rep(" If you wish to handle Paths in excess of this length.. Contact Me ",0x4f,10);
			}
			else
			cprintf("%s",The_Path);

			kout = GetKey();

			if(strlen(The_Path) >= 70)
				kout = ESC;

			if(kout == ESC)
				break;

			if(kout != CR)
			{  
				strset(The_Path,'\0');
				strcpy(tmp,buf);
			}
		}
	}
	if(matchfound == 0 || kout != CR && kout != ESC)
	{
		gotoxy(2,6);
		WriteVideo(3,1,3,"     [   The required programme was not found !  ]     ",0x1e,0);
		Beep(100,200);
		delay(1000);
	}

	fclose(datafile);

	if(kout == CR && matchfound != 0) 
	{
		strcpy(The_Path,tmp);
		The_Path[(strlen(The_Path))-1] = '\0';
		strset(tmp,'\0');
		return(1);
	}
	else
	return(0);
}

static void ShowPath(msy)
{
	char s[ZPL];
	strcpy(s,nd.pathmot[msy-1]);
	strcat(s,"]");
	WriteVideo(1,3,15,"[Path=",0x1f,0);
	WriteVideo(1,9,15,"ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ",0x1f+0x01,0);
	if((strlen(nd.pathmot[msy-1]) > 47))
  {
    s[48] = '\0';
    s[47] = ']';
    s[46] = '.';
    s[45] = '.';
    s[44] = '.';
	}
	WriteVideo(1,9,15,s,0x1f,0);
	ShowRecords();
}

void StuffBuffer(char *s)
{
	int size,i;
	size = strlen(s);
	for(i = 0; i < size; i++)
	StuffKey(s[i]);
}

void LabelWindows(void)
{
	int lx = 0;
	static int si;
	int i = 0;
	int k = 0;

	switch(GlobalKey)
  {
    case 'D':
    case 'd':
		{	
			lx = SubMenu(13,3,"[ Execute A Dos Commands? ]",dsm,dsmh,0x3f3f,0x3f06);

			Edit = 1;

			switch(lx)
			{
				case 0:  strset(myargs,'\0');       break;
				case 1:  strcpy(myargs,"COPY ");	  break;
				case 2:  strcpy(myargs,"MOVE ");    break;
				case 3:  strcpy(myargs,"DEL ");     break;
				case 4:  strcpy(myargs,"DEL ");     break;
				case 5:  strcpy(myargs,"MSBACKUP"); break;
				case 6:  strcpy(myargs,"DEFRAG "); 	break;
				case 7:  strcpy(myargs,"COPY A:\\*.* A:"); break;
				case 8:  strcpy(myargs,"EDIT ");         break;
				case 9:  strcpy(myargs,"MEM /C/P");   	 break;
				case 10: strcpy(myargs,"FORMAT A: /S");  break;
				case 11: strcpy(myargs,"MSAV");          break;
				case 12: strcpy(myargs,"MSD"); 					 break;
				default: Edit = 0; break;
			}

			if(GlobalKey == ESC)
			{
				Edit = 0;
				strset(myargs,'\0');
				Rep(" ",0x1f,0);
				return;
			}
			DosArg(100); // dosarg called by dos command window
      Rep("",0x1f,0);
      return;
		}

		case 'n':
		case 'N':
		{
			lx = SubMenu( 8,7,"[ DosRun / Navigator Support Files ]",nsm,nsmh,0x3f3b,0x3f06);

			if(GlobalKey == CR)
				RunItNow(lx);
			return;
		}

    case 't':
		case 'T':  
		{
			Busy_Flag = 1;
			fd2 = open(dosruntec, O_RDWR | O_CREAT | O_BINARY,S_IWRITE );//create new
			lseek(fd2, Current_TRecord, SEEK_SET);
			read(fd2, &tnd, sizeof tnd);           // read in a group of data
			Current_TRecord = tell(fd2);

      OpenWindow(5,7,75,21," Time Activated Notepad ",0x0f0f,0x0f06);
			CursorOff();
			WriteVideo(3, 2, 2,T1,0x0b,0);
			WriteVideo(3, 2, 3,T2,0x0b,0);
			WriteVideo(3, 2, 4,T3,0x0b,0);

      for(i = 5; i < 14; i++)
        WriteVideo(3, 2, i,T4,0x0b,0);

	    WriteVideo(3, 2,14,TD,0x0b,0);

      RunPrompt("",0x1e);

      read(fd2, &tnd, sizeof tnd);           // read in a group of data
	    Current_TRecord = tell(fd2);

	    for(i = 0; (trec+i)->fcol; i++)
	    {                            
        if((trec+i)->flen == 1)
          if(strcmp((trec+i)->fbuff,"û"))
            strcpy((trec+i)->fbuff,"N");
        WriteVideo(1,((trec+i)->fcol),((trec+i)->frow),((trec+i)->fbuff),((trec+i)->color),0);//((trec+i)->flen)
	    }
			CursorOn();

			if(Message_Waiting != 0)
			{
				strset((trec+Message_Waiting)->fbuff,'\0');
				strcpy((trec+Message_Waiting)->fbuff,"û");
				SeekBeginingCurRcd(fd2);
				write(fd2, &tnd,sizeof tnd);
				CursorOff();
				Beep(600,100);
				Rep("Message Acknowledged...... Press the Esc Key",0x0e,1);//???//
				 CloseWindow();
				 Message_Waiting = 0;
				 Busy_Flag = 1;
				close(fd2);
				return;
			}


			do
			{                            
				if((trec+si)->flen == 8) Rep("Enter the Date eg. ‘e13/06/95‘‘  Press F10 to enter todays date.",0x3b,10);
				if((trec+si)->flen == 5) Rep("Enter a Time (24 Hour Clock) eg. 14:02  ",0x3b,10);
				if((trec+si)->flen >  8) Rep("Enter your Message / Reminder...  ",0x3b,10);
				if((trec+si)->flen == 1) Rep("Enter ‘eN‘‘ to indicates a new message (F9 Reset to N).",0x3b,10);
				WriteVideo(0,1,25," F8 Clear Item,  F9 Clear Line,  Up Arrow = Next,  Down Arrow = Last, ESC Quit ",0x1f,0);

				gotoxy((trec+si)->fcol-1,(trec+si)->frow-1);
				k = EditLine((trec+si)->fbuff,Legal_Text,(trec+si)->flen,(trec+si)->color,1,(trec+si)->type);

				switch(k)
        {
					case CR:        if(si >= 36) break; else si++;      break;

					case UP:
					case CTRLLEFT:  if(si > 0) si--; else (si = 35 - si); break;

					case DOWN:
					case CTRLRIGHT: if(si <  35) si++; else si =  0;    break;

					case PGUP:
					case CTRLUP:    if(si >=  4) si -= 4; break;

					case PGDN:
					case CTRLDOWN:  if(si < 30)  si += 4; break;

					case F9:
										if(k == F9)
										{
											i = si % 4;
											while(i--) { si--; }
											i = 4;
											while(i--)
											{
												strset((trec+si)->fbuff,'\0');
												WriteVideo(1,((trec+si)->fcol),((trec+si)->frow),((trec+si)->fbuff),((trec+si)->color),(trec+si)->flen);
												si++;
											}
											i = 4;
											while(i--) si--;
										}
										break;

					case F8:    strset((trec+si)->fbuff,'\0');
											WriteVideo(1,((trec+si)->fcol),((trec+si)->frow),((trec+si)->fbuff),((trec+si)->color),(trec+si)->flen);
											if( strlen((trec+si)->fbuff) == 1)
												strcpy((trec+si)->fbuff,"N");
											break;

					case F10:   if((trec+si)->flen == 8)
											{
												strset((trec+si)->fbuff,'\0');
												strcpy((trec+si)->fbuff,dateis);
											}
					default:    i = 0; break;
				}
//				Rep("",0x0e,0);

			}while(k != ESC);// || k != F2)

      SeekBeginingCurRcd(fd2);
      write(fd2, &tnd,sizeof tnd);

      CursorOff();
      CloseWindow();
      Message_Waiting = 0;
      Busy_Flag = 1;
      Rep(" ",0x1b,0);
      RunPrompt("",0x1e);
			close(fd2);
		}
  } 
}

void RunItNow(int pos2)
{
	UnhookVectors();
	switch(pos2)
	{
		case 0: spawnl(P_WAIT, "C:\\RUN\\DOSRUNC.EXE", NULL); break;
		case 1: 
    {
			// should close all files just in case ??????//

			//CloseFile(fd1);
			spawnl(P_WAIT, "C:\\RUN\\DOSRUNB.EXE", NULL);
//			fd1 = OpenFile(dosrunrec,O_RDWR | O_CREAT | O_BINARY,SH_DENYNO,S_IWRITE);
//			read(fd2, &tnd, sizeof tnd);
//      StuffKey(ESC);
			GlobalCall = CTRLHOME;
		}
		break;

		case 2:   spawnl(P_WAIT, "C:\\RUN\\DOSRUND.EXE", NULL); break;//
		case 4:   spawnl(P_WAIT, "C:\\RUN\\DOSRUNU.EXE"  , NULL); break;
		case 6:   spawnl(P_WAIT, "C:\\RUN\\LOOK.EXE"    , NULL); break;
		case 7:   spawnl(P_WAIT, "C:\\RUN\\DOSRUNH.EXE"  , NULL); break;
		case 9:   break;//spawnl(P_WAIT, "C:\\RUN\\EXESAFE.EXE" , NULL); break;
		case 100: spawnl(P_WAIT, "C:\\RUN\\DOSRUNH.EXE" , NULL); break;
		default:  break;
	}
	GlobalKey = 0;
	//ClearBiosBuff();
	HookVectors();
}

void DosArg(int i)
{
	if(strrchr(nd.pathmot[LinePosition-1],'?') || i == 100)
	{
		if(nd.pathmot[LinePosition-1][0] == '?')
		{
			if(i != 100)
			{
				i = 1;// use i to mark the q mark so we can put it back //

				nd.pathmot[LinePosition-1][0] = ' ';
			}

			OpenWindow(3,5,77,7,"[ Enter Dos Command ]",0x4f4f,0x4f06);
			WaitForKey = 1;
		}
		else

		{
			OpenWindow(3,5,77,7,"[ Enter Arguments ]",0x4f4f,0x4f06);
		}

		CursorOn();
		gotoxy(2,1);
		GlobalKey = EditLine(myargs,Legal_Text,70,0x4f,Edit,0);//0
		CursorOff();

		if(GlobalKey == CR)
		{
			if(i != 100)
				nd.pathmot[LinePosition][(strlen(nd.pathmot[LinePosition-1])-2)]='\0';//get rid of ?//
				strcpy(myargs,(strupr(myargs)));
		}
                		else
		{
			if(i == 1) //if tl = 1 the put back ? //
			nd.pathmot[LinePosition-1][0] = '?';// put back ? //
			Rep(" ",0x1f,0);
			strset(myargs,'\0');
			CloseWindow();
			i = 0;
			return;
		}
		Rep(" ",0x1f,0);
		CloseWindow();
	}
	if(strstr(myargs,"MORE") || strstr(myargs,"/P"))    WaitForKey = 1;
}

//////////////////////////////////////////////////////////


void GetC(void)
{
	int i = 0;
	char c  = ' ';
	char cc = ' ';
	char lop[20];

	while(i < strlen(fnd.qaz)) 
	{
		c = fnd.qaz[i];

		if(c < 1)
			c = c + 256;

		cc =  (c - 128) - i;
		cc = cc / 2 ; 

		if(cc < 0)
			cc = cc + 128;

		lop[i++] = cc;
	}
	lop[i] = '\0';
	strcpy(mott,lop);
}

void MatchMouse(void)
{

	if(Mouse_Buttons == 2)
	{	cprintf("\rX = %d Y = %d",Mouse_X*16,Mouse_Y*8); delay(2000); }

	if(Mouse_X*16 >= 8192 && Mouse_X*16 <= 9856)
	switch(Mouse_Y*8)
	{
		case 192:  StuffKey('V'); return;
		case 320:  StuffKey('D'); return;
		case 448:  StuffKey('L'); return;
		case 576:  StuffKey('F'); return;
		case 704:  StuffKey('R'); return;
    case 832:  StuffKey('P'); return;
    case 960:  StuffKey('M'); return;
    case 1088: StuffKey('T'); return;
		default:   break;
	}

	if(Mouse_Y == 0)
  {
/*
		if(Mouse_X >= 256 && Mouse_X <= 1408)
			StuffKey(ALTP);
    else
		if(Mouse_X >= 2176 && Mouse_X <= 2688)
			StuffKey(ALTM);
    else
		if(Mouse_X >= 3456 && Mouse_X <= 4480)
			StuffKey(ALTU);
    else
		if(Mouse_X >= 5248 && Mouse_X <= 5632)
*/
			StuffKey(ALTP);
  }
}
int CheckMouse(void)
{
	if(Mouse_Detected)
	{
		Mouse_Lasty = wherey();
		Mouse_Lastx = wherex();
		GetMouseAction(); 

		if(Mouse_Buttons != 0)
			StuffKey(255);
		return(255);
	}
	return(0);
}

void ListMenus(void)
{
  i = ListWindowFile();
  i++;
  lseek(fd1, i*sizeof nd, SEEK_SET);
  SeekBeginingCurRcd(fd1);
  NextRecord();
}

int ListWindowFile(void)
{
  static char List[50][61];
  int linecount = 0;
  int cur; 
  int maxlength  = 15;
  int start = 0; 
	int k = 0;
  int i = 0;
  int fsize = 0;
	int updateflag = 1;
	int ii = 0;
	int x = 9;
	int endx  = 0;
	int startx = 15;
	int starty = 3;

	FirstRecord();
	while(linecount < Record_Count)
	{
		strcpy(List[i++],nd.menuname);
		ii = strlen(List[i-1]);
		fsize = fsize + ii;
		if(ii > endx)  endx = ii;
		linecount++;
		if(i == 50)
			break;
		read(fd1, &nd, sizeof nd);           // read in a group of data
	}
	endx++;
	cur = 0;

	if(endx > 70)
		endx = (78 - startx);

	OpenWindow(startx,starty,endx+startx+4,starty+maxlength+2,"[ Menus ]",0x3f3b,0x3f06);

	CursorOff();
 do
 {
	 updateflag = 1;
	 switch(k)
	 {
		 case DOWN: if(cur < maxlength && cur + start < linecount -1)
								{
									WriteVideo(3,1,cur+2,List[cur+start],0x3f,0);
									cur++; updateflag = 0;
								}
								else
								if(cur + start < linecount -1) { start++; }
								else
								if(cur == maxlength && start > linecount - maxlength) { updateflag = 1; }
								break;

		 case UP:   if(cur >  0 )
                { 
									WriteVideo(3,1,cur+2,List[cur+start],0x3f,0);
									cur--; updateflag = 1;
								}
                else 
                if(cur == 0 && start > 0) { start--; }
                break; 



     case PGDN: if(start + maxlength < linecount - maxlength) start = start + maxlength; 
                else 
                start = linecount - maxlength - 1; 
                break;

     case PGUP: if(start > maxlength)  start = start - maxlength; 
                else 
                start =  0;
                break;

     case CTRLHOME: start = 0; break;
     case CTRLEND:  start = linecount - maxlength - 1; break;

			 case HOME: if(cur == 0) updateflag = 0;
									else
									cur = 0;  break;
			case END:  	if(cur == maxlength) updateflag = 0;
									else
									//cur = maxlength; break;
									cur = linecount-1; break;
			case CR:   {
										CloseWindow();
										CursorOn();
										for(i = 0; i <= linecount; i++)
										strset(List[i],'\0');
										return(cur+start);
									}

			default:   //FindLine(linecount, maxlength, cur, k, start);
								break;
		}

		x = start;
		if(updateflag)
		{
			clrscr();
			for(i = 0; i <= maxlength; i++)
			{
				WriteVideo(3,1,i+2,List[x++],0x3f,0);
			}
		}
		WriteVideo(3,1,cur+2,List[start+cur],0x4f,0);
		k = GetKey();
	}while(k != ESC);
	CloseWindow();
	CursorOn();

  for(i = 0; i <= linecount; i++)
		strset(List[i],'\0');
	return(-1);
}

void SaveScrn(void)
{
  int cnt = 0;
  static i;
  UnhookVectors();
  OpenWindow(1,1,80,25,"",0x0000,0x0000);
  xt = 0;
  do
  {
    cnt++;
    textcolor(rand() % 16);
  	gotoxy(rand() % 80, rand() % 25);

  	cprintf(".");
    delay(i = (rand() % 950));
		if(i < 500)
			delay(1500);
	 if(cnt == 500)
   {  clrscr(); cnt = 0; }
  }while(bioskey(1) == 0);
  ClearBiosBuff();
  CloseWindow();
  HookVectors();
}

int MenuSummary(void)
{
	int dosrunhelphandle;
	int i = 0;
	int k = 0;
	char ch;

	dosrunhelphandle = open(dosrunhelp, O_RDWR | O_CREAT | O_BINARY,S_IWRITE);
	if(dosrunhelphandle == -1) return(2477);

	if(FileIndexCount == 0)
	{
		if(ReadFileIndex(dosrunhelp) == -1)
		{
			if(CreateFileIndex(dosrunhelp,dosrunhelphandle) == -1)
			{
				Rep("ERROR Creating Index File... Press any key to exit help ",0x4f+BLINK,1);
				close(dosrunhelphandle);
				return(-1);
			}
			else
			if(ReadFileIndex(dosrunhelp) == -1)
			{
				Rep("ERROR Reading Index File... Press any key to exit help ",0x4f+BLINK,1);
				close(dosrunhelphandle);
				return(-1);
			}
		}
	}

	OpenWindow(1,2,80,24,"[ Special Keys Used By DOSRUN Version 7.01] ",0x7f7f,0x7f01);
	lseek(dosrunhelphandle,FileIndex[0], SEEK_SET);

	do
	{
		if(GetJustification(dosrunhelphandle))
			ch = WriteHelpLine(dosrunhelphandle);
		else
		 break;

		read(dosrunhelphandle,&ch,1);
		if(ch != '“')
			lseek(dosrunhelphandle,-2, SEEK_CUR);
		else
		{
			k = GetKey();
			clrscr();
			switch(k)
			{
				case DOWN:  if(i < FileIndexCount-1) i++; else i = 0; break;
				case UP:    if(i > 0) i--; else i = FileIndexCount-1; break;
				case ESC:   break;
				default:    StuffKey(ESC); break;
			}
			lseek(dosrunhelphandle,FileIndex[i], SEEK_SET);
		}
	}while(k != ESC);//while(!eof(dosrunhelphandle) && k != ESC);

	close(dosrunhelphandle);
	CloseWindow();
	RunPrompt("",0x1e);
	return(OK);
}

int GetJustification(int handle)
{
	char ch;
	char wh[3];

	read(handle,&ch,1);
	if(ch != '”')
	{ cprintf("\n\rError Index Mark = [%c]",ch); GetKey(); return(0); }

	read(handle,wh,1);
	wjust = atoi(wh);
	read(handle,wh,2);
	xjust = atoi(wh);
	read(handle,wh,2);
	yjust = atoi(wh);
	return(1);
}

char WriteHelpLine(int handle)
{
	char ch;
	char linebuffer[91] = "";
	int count = 0;
	int colr=0x7f;

	do
	{ read(handle,&ch,1);
		linebuffer[count++] = ch;
		if(count >= 90)
			count = 0;
	} while(ch != '”');

	linebuffer[count-1] = '\0';
	WriteVideo(wjust,xjust,yjust,linebuffer,colr,0);
	count = 0;
	return(ch);
}

void WindowsOptions(int num)
{
	int i = 0;
	char *ptr;
	char name[13];
	if( strstr(nd.pathmot[num-1],"\\ WIN ") || strstr(nd.pathmot[num-1],"WIN.COM") != NULL)
	{
		Rep("This Files Path is already Processed for Windows..... Press any key..",0x0f,1);
		return;
  }

	OpenWindow(20,6,60,8," Windows 3.XX Options ",0x1f1b,0x1f06);
	Rep("If this is a Windows programme, Dosrun will tell Windows to execute it directly",0x0f,10);
	cprintf("  Is this a Windows Programme ? Y/N" );
	i = GetKey();
	if(i == 'Y' || i == 'y')
	{
		ptr = strrchr(nd.pathmot[num-1],'\\');   // find last /
		strcpy(name,++ptr);
		--ptr;

		strcpy(ptr,'\0');
		strcat(nd.pathmot[num-1],"\\ WIN ");
		strcat(nd.pathmot[num-1],name);
	}
	CloseWindow();
	Rep("",0,0);
	GlobalKey = i;
}

void Update(void)
{
	if(tick > 18)
	{
		ShowTimeTop();
		tick = 0;
		xt++;
	}

	if(xt > 200)
	{
		SaveScrn();
		xt = 0;
	}
}
