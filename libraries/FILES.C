//December 2nd/3rd 1996//
#include <conio.h>
#include <string.h>
#include <io.h>
#include <files.h>
#include <keyios.h>
#include <xwindows.h>


#define MAXOPEN 25
#define MAXFL   25

static int thisone;
static int opencount;

struct FilesOpenStructure
{
	char Fname[MAXOPEN][80];
	int  Fhandle[MAXOPEN];
}FO;

static struct FileLockData
{
 int  FLha[MAXFL];     // handle
 int  FLst[MAXFL];     // status
 long FLof[MAXFL];     // offset
 long FLsz[MAXFL];     // sizze
 int  FLow[MAXFL];     // owner
 char FLna[MAXFL][81]; // name
}FLD;



extern void Rep(char *, int, int);
extern CallCritError(int);

int OpenFile(char *fname, int access, int shf,unsigned int mode)
{
	int i = 0;

	if(opencount >= MAXOPEN)
	{
		Rep("No More Handles... Please Increase MAXOPEN",0x4f,1);
		return(-1);
	}
	for(i = 0; i < MAXOPEN; i++)
	{
		if(strcmp(FO.Fname[i],fname) == 0)
		{
			Rep("This File is already Open... Press any key ",0x4f,1);
			return(FO.Fhandle[i]);
		}
	}
	strcpy(FO.Fname[opencount],fname);
	FO.Fhandle[opencount]  = sopen(fname,access,shf,mode);//create new

	if(FO.Fhandle[opencount] == -1)
	{
		strset(FO.Fname[opencount],'\0');
		FO.Fhandle[opencount] = 0;
		CallCritError(FO.Fhandle[opencount]);
	}
	opencount++;
	return(FO.Fhandle[opencount-1]);
}

void CloseFile(int h)
{
	int i = 0;

	if(h != 0)
	{
		close(h);

		for(i = 0; i < MAXOPEN; i++)
		{
			if(FO.Fhandle[i] == h)
			{
//				close(FO.Fhandle[i]);
				FO.Fhandle[i] = 0;
				opencount--;
				return;
			}
		}
	}
/*
	else
	for(i = 0; i < MAXOPEN; i++)
	{
		if(strcmp(FO.Fname[i],fname) == 0)
		{
			close(FO.Fhandle[i]);
			FO.Fhandle[i] = 0;
			opencount--;
			return;
		}
	}
*/
	Rep("You are attempting to Close a File that is Not Opened",0x4f,1);
}



void CloseAllFiles(void)
{
 int i = 0;
	for(i = 0; i < MAXOPEN; i++)
	{
		if(FO.Fhandle[i] > 0)
			close(FO.Fhandle[i]);
	}
}

char *GetFileName(int h)
{
	int i = 0;
	if(h != 0)
	{
		for(i = 0; i < MAXOPEN; i++)
		{
			if(FO.Fhandle[i] == h)
				return(FO.Fname[i]);
		}
	}
	return("");
}




int LockFile(int fh, long offset, long sizze)
{

	int status;

gotoxy(1,22);
cprintf("\n\rCall to LockFile: Handle %d Offset %Ld Size %Ld",fh,offset,sizze);
cprintf("\n\rCall to LockFile: Handle %d Offset %d Size %d",fh,offset,sizze);
GetKey();

	status = lock(fh,offset,sizze);

	if(status == 0)
	{
		FLD.FLha[thisone] = fh;
		FLD.FLof[thisone] = offset;
		FLD.FLst[thisone] = 1;
		FLD.FLsz[thisone] = sizze;
		FLD.FLow[thisone] = 0;
		strcpy(FLD.FLna[thisone],GetFileName(fh));

		gotoxy(1,2);cprintf("This = %d Locking [%s] at %4Ld %4Ld",thisone,FLD.FLna[thisone],FLD.FLof[thisone],FLD.FLsz[thisone]); GetKey();
		thisone++;
	}
	else
	{
		OpenWindow(10,5,70,8," ",0x0f0b,0x0f02);
		cprintf("\n\r Unable to Lock File... In Use...");
		GetKey();
		CloseWindow();
	}

	return(status);
}

int UnLockFile(int fh, long offset, long sizze)
{
	int status;
	int temp = 0;

	status = unlock(fh,offset, sizze);

	if(status == 0)
	{

		while(fh != FLD.FLha[temp++])
		{	if(temp > MAXFL) return(-2); }


		if(FLD.FLst[temp] == 1)
		{
cprintf("\n\rUnLocking [%s] at %Ld",FLD.FLna[temp],FLD.FLof[temp]); GetKey();
			FLD.FLha[temp] = 0;
			FLD.FLof[temp] = 0L;
			FLD.FLsz[temp] = 0L;
			FLD.FLst[temp] = 0;
			FLD.FLow[temp] = 0;
			strset(FLD.FLna[temp],'\0');
			thisone--;
		}
	}
  else
	{
		OpenWindow(10,5,70,8," ",0x0f0b,0x0f02);
		cprintf("\n\r Unable to UnLock File... In Use...");
		GetKey();
		CloseWindow();
	}
	return(status);
}

void ShowLockedFiles(void)
{

	int temp = 0;

	ClearBiosBuff();
	OpenWindow(2,2,78,15," File Locked Information ",0x7f7b,0x7f02);

	if(thisone == 0)
	{
		cprintf(" No Files Locked... Press any Key "); GetKey(); CloseWindow(); return;
	}
	cprintf("\n\r  File Name      Handle     Offset     Size     Owner     Status");
	while(temp < thisone)
	{

		cprintf("\n\r %12s",FLD.FLna[temp]);
		cprintf("      %4d     %6Ld   %6Ld      %4d ",FLD.FLha[temp],FLD.FLof[temp],FLD.FLsz[temp],FLD.FLow[temp]);

		if(FLD.FLst[temp] == 0) cprintf("    Unlocked"); else cprintf("    Locked");
		temp++;
	}
	GetKey();
	CloseWindow();
}
