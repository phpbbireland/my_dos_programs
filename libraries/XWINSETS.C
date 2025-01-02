// winset.c  seperates open and close process

#include <conio.h>
#include <alloc.h>
#include <keyios.h>
#include <stdlib.h>


struct text_info info;
char *buffer;

// open_process 
void OpenProcess(void)
{
	int mode;
	int buffsize;
	
	gettextinfo(&info);

	mode = info.currmode;

	if(mode < 0 || mode > 20)
	{
		cprintf("%d MODE NOT SUPPORTED",mode); // NOT SUPPORTED");
		exit(-1);
	}                     

	buffsize = 2*(info.screenheight)*(info.screenwidth);
	if((buffer = (char *) malloc(buffsize)) == NULL)
	{
		cprintf("MEMORY ALLOCATION ERROR");
		GetKey();
		exit(-1);
	}
	if(!gettext(info.winleft,info.wintop,info.winright,info.winbottom,buffer))
	{
		cprintf("GETEXT ERROR");
		GetKey();
		exit(-1);
	}
}

// close_process
void CloseProcess(void)
{
	textmode(info.currmode);
	if(!puttext(info.winleft,info.wintop,info.winright,info.winbottom,buffer))
	{
		cprintf("PUTTEXT ERROR");
		GetKey();
		exit(-1);
	}
	textattr(info.attribute);
	gotoxy(info.curx,info.cury);
	free((void *) buffer);
}

