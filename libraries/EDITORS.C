//*************************************//
//  all edit text functions  sep 96    //
//*************************************//

#include <keyios.h>
#include <editors.h>
#include <cursors.h>

#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <mem.h>
#include <ctype.h>

#define FALSE 0

int EditLine(char *s,char *legal,int maxlength, int attr,int edit, int type)
{
	int c = 0, len, pos, insert = FALSE;
  static int lastc;
	int x,y,xx;

	x = wherex(); 
	y = wherey();

  len = strlen(s);
  pos = len;

	if(type > 3)
	{  cprintf("ERROR IN TYPE: 0 = Text 1 = Num 2 = Date&Time by size"); GetKey(); return(-1); }

	textcolor(attr);
	textbackground(attr>>4);

//	maxlength--;

	if(edit == 1) 
	{
		if(!strcmp(s," ")) 
			s[0] = '\0'; 
	}
	else
	{
		strnset(s,'\0',strlen(s));//maxlength);
		len = strlen(s);
		pos = len;
	}
	pos = pos + x - 1;

	do
	{
		gotoxy(x,y);
		cprintf("%s",s);
		gotoxy(pos+1,y);

		c=GetKey();

		switch(c)
		{

		case '\b': 
			if(pos > x -1)
			{
				xx = wherex();
        gotoxy(len+x-1,y); 
        cputs(" "); //cprintf("%d",len);
				gotoxy(xx,y);

        if(s[pos - x + 1] == '\0')// added for end character correction
        {
          s[pos - x] = ' ';
          s[pos - x + 1] = '\0';

        }
        else
				movmem(&s[pos - x + 1], &s[pos - x ], len - pos+x+1); 
				len--;
				pos--;
			}
			break;

		case LEFT: 
			if(pos > x-1)  // Left Arrow
				pos--;
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
			ChangeCursor(insert);
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
			if(type == 1 || type == 2)
				if(isdigit(c) || c == '.' || c == '-' || c == '/' || c == ':')
					;
			else
				break;
			if(isprint(c) && pos-x+1 <= len)
			{ 
				if (((legal[0] == 0) || (strchr(legal, c) != NULL)) && ((c >= ' ') && (c <= '~')) && (pos-x < maxlength-1))//-1 = last char
				{
					if (insert)
					{
            if(len > maxlength)
							break;
						if(pos-x < maxlength-1) // stops it putting a space at end //
						memmove(&s[pos - x + 2 ], &s[pos - x + 1], len - pos + 1 + x ); len++;
					}
					else

					if (pos-x+1 >= len)
						len++;

          if(lastc == ' ' || lastc == '\'' || len == 1 && type == 0) c = toupper(c);//
						s[(pos++)-x+1] = c;

					lastc = c;

					if(type == 2 && maxlength == 5)//TIME//
					{
						if((len == 1 && c > '2') || (len == 2 && c > '4'))
							StuffKey(LEFT);
						else
						if(len == 4 && c > '5')
							StuffKey(LEFT);
						else
						if(len == 2)
							StuffKey(':');
					}

					if(type == 2 && maxlength == 8)
					{
						if(len == 2 || len == 5)
							StuffKey('/');
					}
				}
				else
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
  
	ChangeCursor(FALSE);

  s[len]= '\0';
	return(c);
}

int  EditI(int *n, int edit, int attr)
{
	int x = 0;
	char buff[12] = "";
	itoa(*n,buff,10);
	x = EditLine(buff,"",6,attr,edit,1);//number
	*n = atoi(buff);
	cprintf("\n\r[%d]\n\r",*n);
	return(x);
}

int  EditL(long *n, int edit, int attr)
{
	int x = 0;
	char buff[11] = "";
	ltoa(*n,buff,10);
	x = EditLine(buff,"",10,attr,edit,1);//number
	*n = atol(buff);
	return(x);
}

int  EditUL(unsigned long *n, int edit, int attr)
{
	int x = 0;
	char *ep;
	char buff[31] = "";
	ultoa(*n,buff,10);
	x = EditLine(buff,"",30,attr,edit,1);//number
	*n = strtoul(buff,&ep,10);
	return(x);
}

int  EditF(float *n, int edit, int attr)// rounded to 2 places //
{
	int x = 0;
	char *buff = "                               ";
	int dec, sign;
	buff = fcvt(*n, 2, &dec, &sign);
	x = EditLine(buff,"",30,attr,edit,1);//number
	*n = atof(buff);
	return(x);
}


/*
// text line editor //// allows time and date and text //
void main(void)
{

 int i = 123;
 long l = 123456L;
 unsigned long ul = 123456789L;
 float f = 3.14;
 double d = 243444;
 long double ld = 123456780;

 char ELB[80];
 char Legal_Text[] ="";
 strset(ELB,'\0');
 clrscr();
 cprintf("Start\n\r");


 EditLine(ELB,Legal_Text,8,0x1f,1,2);//date
 cprintf("%s\n\r",ELB); GetKey();

 EditLine(ELB,Legal_Text,5,0x1f,0,2);//date
 cprintf("\n\r%s\n\r",ELB);GetKey();

 EditLine(ELB,Legal_Text,10,0x1f,0,1);//number
 cprintf("\n\r%s\n\r",ELB);GetKey();

 EditLine(ELB,Legal_Text,13,0x1f,0,0);//text
 cprintf("\n\r%s\n\r",ELB);GetKey();


 EditI(&i,0,0x1f);
 cprintf("\n\rInt Now = %d\n\r",i);

 EditL(&l,0,0x1f);
 cprintf("\n\rLong Now = %ld\n\r",l);

 EditUL(&ul,0,0x1f);
 cprintf("\n\rUnsigned Long = %lu\n\r",ul);
 GetKey();

 EditF(&f,0,0x4f);
 cprintf("\n\rNow = [%f] [%.2f]\n\r",f,f);
 GetKey();

 EditD(&d,0,0x4f);
 cprintf("\n\rNow = [%f] [%.2f]\n\r",f,f);
 GetKey();

 EditUD(&ld,0,0x4f);
 cprintf("\n\rNow = [%f] [%.2f]\n\r",f,f);
 GetKey();
}

*/
