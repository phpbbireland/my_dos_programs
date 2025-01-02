#include <dos.h>
#include <keyios.h>
#include <bios.h>
#include <stdio.h>
#include <conio.h>
#include <interupt.h>
#include <mouses.h>

extern int GlobalKey;

static int far *nextoff =  MK_FP(0x40,0x1a);
static int far *nexton   = MK_FP(0x40,0x1c);

void ClearBiosBuff(void)
{
  *nextoff = *nexton;
}

int GetKey(void)
{

	int c;
	while(bioskey(1) == 0)
	{
		geninterrupt(0x28);
		Update();         // used to update a clock via static global tick //
								// all main file must have this function even it //
								// does nothing. original in chase.c             //
		if(GetKeys())
		{
			GlobalKey = 255;
			return(255);
		}
	}

	if(((c = bioskey(0)) & 255) == 0)
	c = (c >> 8) | 0x80;
	return(c & 255);
}


int GetKeys(void)
{
	if(Mouse_Detected)
	{
		Mouse_Lasty = wherey();
		Mouse_Lastx = wherex();
		GetMouseAction(); 

		if(Mouse_Buttons != 0)
			return(Mouse_Buttons);
	}
	return(0);
}



void StuffKey(int k)
{
  if(k & 128) k = (k << 8) & 0x7f00;
  if(*nexton + 2 == *nextoff || (*nexton == 0x3c && *nextoff == 0x1e))
  return;
  poke(0x40, *nexton, k);
  *nexton += 2;
  if(*nexton == 0x3e) *nexton = 0x1e;
}


// add these later //
/*
static char *nxton  = Bios_Buffer;
static char *nxtoff = Bios_Buffer;
static int rate = 0;
static void interrupt (*oldkb)(void) = NULL;
static void interrupt newkb(IREGS);
static void InsertKey(int);
static void NextBuff(void);
static char Bios_Buffer[256];
*/






/*
static void InsertKey(int c)
{
	if(nxton+1 == nxtoff || (nxton+1 == Bios_Buffer+256 && nxtoff == Bios_Buffer))
	 return;
	*nxton++ = c;
	if(nxton == Bios_Buffer+256)
	 nxton = Bios_Buffer;
}

void AutoType(char *s)
{
	while(*s)
	InsertKey(*s++);
	if(oldkb == NULL)
	{
		oldkb = getvect(0x16);
		setvect(0x16,newkb);
	}
}
*/

