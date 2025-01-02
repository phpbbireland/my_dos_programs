
/*
int GetAllDisks(void)
{	
	int save,disks;
	save = getdisk();
	disks = setdisk(save);
	return(disks);
}
*/


/*
void MouseCase(void) 
// match mouse position to action //
{
	if(Mouse_Detected ==0)
  return;

	if(Mouse_Buttons == 0)
	return;

	if(Mouse_X == 56 && Mouse_Y == 16)
	{ 
		HelpNo = ALTM;
		StuffKey(ALTM);
		CursorOff();
		return; 
	}

	if(Mouse_X >= 120 && Mouse_X <= 512) // lines only //
		switch(Mouse_Y)
		{
			case 32:  gotoxy(11,2); break;
			case 40:  gotoxy(11,3); break;
			case 48:  gotoxy(11,4); break;
			case 56:  gotoxy(11,5);break;
			case 64:  gotoxy(11,6);break;
			case 72:  gotoxy(11,7);break;
			case 80:  gotoxy(11,8);break;
			case 88:  gotoxy(11,9);break;
			case 96:  gotoxy(11,10);break;
			case 104: gotoxy(11,11);break;
			default:  break;
		}

		switch(Mouse_X)
		{
			case  32: if(Mouse_Y == 128) StuffKey(PGUP); break;
			case 592: if(Mouse_Y == 128) StuffKey(PGDN);
								//if(Mouse_Y ==  16) StuffKey(F3);   break;
			case 616: if(Mouse_Y == 192) StuffKey('?');  break;
			default:  break;
		}

	CursorOff();
}
*/




/*
void PointTo(int sx,int sy, int attr)
{
  int far *farvideoptr;
	farvideoptr = (int far *)VGAP0 // colour address //
	sx--;
  attr = attr << 8;
  sy = sy * 80;
  *(farvideoptr+sx+sy) = 17 + attr+256;   //16 = > 17 = <//
}
*/


//static void DeleteItem(RECORDENTRYFIELD *,int);
//static void BlankALine(RECORDENTRYFIELD *);
//static int  GetRecordLine(RECORDENTRYFIELD *);
//static void EditRecord(RECORDENTRYFIELD *);
//static void AddNewRecord(RECORDENTRYFIELD *);
//int  FindIt(char *);

//int  DataEntry(RECORDENTRYFIELD *);




*********************************

FILE *datafile;
FILE *datafile;

  static start;
  static end;
  static cur;
  int xx = 9;
  int k = 0;
  char List[100][82] = {""};





//

 i = 0;
 while(fgets(List[i++],81,datafile))
 {
   if(i > 50)
     break;

 }

 OpenWindow(10,5,60,18," List Window ",0x0f0b,0x0f06);

 start = 0;

 do
 {
   switch(k)
   {
     case DOWN: if(cur < 11)  cur++; 
                else 
                if(cur == 11 && start < 39)
                { 
                  start++; end++; 
                } break; 

     case UP:   if(cur >  0)  cur--; 
                else 
                if(cur == 0)
                { 
                  start--; end--; 
                } break; 

     case PGDN: if(end < 50-12) start = start + 11; else start = 40; break;
     case PGUP: if(start > 11)  start = start - 12; else start =  0;break;
     case HOME: start = 0;  break;
     case END:  start = 39; break;
     default:   break;
   }

   xx = start;

   cur = start;

   for(i = 0; i < 12; i++)
   {  
     WriteVideo(1,2,i+2," ",0x0f,49);
     WriteVideo(1,3,i+2,List[xx++],0x0f,0);
   }
   WriteVideo(1,3,cur+2,List[xx],0x4f,0);

  k = GetKey();

 }while(k != ESC);

 CloseWindow();


***********************************************************

