
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>


/*					MACROS					*/
#define streql(x,y) (strcmp(x,y)==0)

#define TRUE  1
#define FALSE 0
#define FIRST 0
#define NEXT  1

#define AFILE     0x01
#define DIR       0x10
#define NORHID    0x06
#define NORHIDSYS 0x06



void set_dta(char *);
int  search(char *, int ,int);

char *LoopSearch(char *, char *);



char *LoopSearch(char *dir, char *name)
{
	int fnd  = 0;
	int flag = 0;

	char dirname [256]; 
	char filename[256]; 
	char dta[43];
  char *ptr

	union REGS regs;

	sprintf(filename,"%s\\%s",dir,name);

	regs.h.ah = 26;
	regs.x.dx = (int)ptr;
	intdos(&regs,&regs);


	flag = FIRST;

	while(fnd = search(filename,flag,NORHID))
	{
		flag = NEXT;
	}

	if(fnd == 0)
	{
		sprintf(filename,"%c:\%s\\%s\n\r",disk+'A',dir,dta+30);

		return(filename);
	}




	sprintf(filename,"%s\\*.",dir);


	flag = FIRST;

	while(search(filename,flag,DIR))
	{
		flag = NEXT;


		if(!streql(".",dta+30) && !streql("..",dta+30))
		{
			sprintf(dirname,"%s\\%s",dir,dta+30);
			LoopSearch(dirname,name);
		}

		set_dta(dta);
	}
	return(filename);

}




int search(char *fname, int flag, int type)
{
	union REGS regs;

	regs.h.ah = 0x4e + flag;	/* ah = 4E for find first & 4F for next */
	regs.x.cx = type;           /* cx = attribute to use ?? */
	regs.x.dx = (int)fname;
	intdos(&regs,&regs);
	if(regs.x.cflag == 1) return(FALSE);
	return(TRUE);

}

/*					SETUP DTA					*/

void set_dta(char *ptr)
{
	union REGS regs;
	regs.h.ah = 26;
	regs.x.dx = (int)ptr;
	intdos(&regs,&regs);
}



struct fcblock {
  char      ff_reserved[21];
  char      ff_attrib;
  unsigned  ff_ftime;
  unsigned  ff_fdate;
  long      ff_fsize;
  char      ff_name[13];
};


struct fcb {
  char   fcb_drive;
  char   fcb_name[8],  fcb_ext[3];
  short  fcb_curblk,   fcb_recsize;
  long   fcb_filsize;
  short  fcb_date;
  char   fcb_resv[10], fcb_currec;
  long   fcb_random;
};