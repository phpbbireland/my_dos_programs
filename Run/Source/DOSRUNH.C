//dosrunh.exe//

void F1function(void)
{
	char ss[25];
	char *HelpBuffer;
	int cntr = 0;
			
  if(HelpNo == F1)
  {
    StuffKey(CF7);
    return;
  }
	if(INMENU)
	{
		HelpNo = MenuMatches();
	}

	HelpBuffer= malloc(81);

	fseek(doshelp, 0L, SEEK_SET);

	itoa(HelpNo,ss,10);

	i = 3;
	SaveCursor();
	OpenWindow(5, 6,75,22," Help Requested ",0x0f0f,0x0401);

//  cprintf("[%d]",HelpNo); delay(100);

	while(fgets(HelpBuffer,81,doshelp) != NULL)
	{
   	if(strstr(HelpBuffer,ss) != NULL)
      break;
	}

	CursorOff();

	while(fgets(HelpBuffer,81,doshelp) != NULL)
	{
		if(strstr(HelpBuffer,"*") != NULL)
			break;
		if(strstr(HelpBuffer,"|") != NULL)
		{	cntr = 2; strnset(HelpBuffer,' ',sizeof(char));}
		else cntr = 0;

   	if(i > 16)
		{
			Rep("Press any key for Next Help Page",0x0f+BLINK,1);
			clrscr();
      	Rep(" ",0x1f,0);
			i = 3;
		}
		else
		if(strlen(HelpBuffer) > 1)
    {
//      cprintf("[%d] %s",HelpNo,HelpBuffer); delay(100);
      if(cntr == 0)
			  WriteVideo(1+cntr,3,i++,HelpBuffer,0x0f,0);
      else
        WriteVideo(1+cntr,1,i++,HelpBuffer,0x0f,0);
    }
		else
    	i++;

	}
  
	GetKey();
	CloseWindow();
	free(HelpBuffer);
	RestoreCursor();
}

void MenuSummary(void)
{
  int i = 0x76;
	int k = 0;

	OpenWindow(1,2,80,24,"[ Special Keys Used By DOSRUN Version 7.01] ",0x7f7f,0x7f01);

	while(k != ESC)
	{                               
		WriteVideo(0,5, 4," ‘bF1‘‘   =  The Help given depends on where and when you press F1" ,i,72);
		WriteVideo(0,5, 5," ‘bF2‘‘   =  Add description for your intended programme" ,i,72);
		WriteVideo(0,5, 6," ‘bF3‘‘   =  (Not Asigned)" ,i,72);
		WriteVideo(0,5, 7," ‘bF4‘‘   =  Delete current description and programme (Same as Del)",i,72);
		WriteVideo(0,5, 8," ‘bF5‘‘   =  Edit the current description" ,i,72);
		WriteVideo(0,5, 9," ‘bF6‘‘   =  Find a Programme or Menu. Search all Menus for a match" ,i,72);
		WriteVideo(0,5,10," ‘bF7‘‘   =  Move a Programme to another Line or Menu......" ,i,72);
		WriteVideo(0,5,11," ‘bF8‘‘   =  Protect a programme/Menu with a Password......" ,i,72);
		WriteVideo(0,5,12," ‘bF9‘‘   =  (Path Edit) Edit an existng Path or add path/switch manually." ,i,72);
		WriteVideo(0,5,13," ‘bF10‘‘  =  Add a programme (path) Automatic Search " ,i,72);

		WriteVideo(0,5,15," ‘bINS‘‘  =  Add / Change description of programmes",i,72);
		WriteVideo(0,5,16," ‘bDel‘‘  =  Delete the current Programme & description",i,72);
		WriteVideo(0,5,17," ‘bHOME‘‘ =  Goto First line (description) of current Menu",i,72);
		WriteVideo(0,5,18," ‘bEND‘‘  =  Goto the last line (description) of current Menu.",i,72);
		WriteVideo(0,5,19," ‘bESC‘‘  =  Quit Programme",i,72);

		WriteVideo(0,3,21," ‘bPGUP, Left Arrow‘‘  = Goto Previous Menu  ‘bPGDN, Right Arrow‘‘ = Goto Next Menu ",i,0);
		WriteVideo(3,4,22," ‘bCtrl + HOME or END‘‘  = Goto First or Last Menu ",i,0);
		WriteVideo(0,1,25," Press Esc to exit.....  Any key Next Page ",0x3f,80);

	  	k = GetKey();
	  	if(k == ESC)
	    	break;
	  	clrscr();


		WriteVideo(2,1, 4," Menu Options " ,0x1f,0);

		WriteVideo(0,5, 6," ‘bAlt F2‘‘  =  Add another Menu (Allows 10 more programmes)" ,i,72);
		WriteVideo(0,5, 7," ‘bAlt F3‘‘  =  Clear" ,i,72);
		WriteVideo(0,5, 8," ‘bAlt F4‘‘  =  Delete current Menu",i,72);
		WriteVideo(0,5, 9," ‘bAlt F5‘‘  =  Edit the Menu Name" ,i,72);
		WriteVideo(0,5,10," ‘bAlt F6‘‘  =  Find a Programme or Menu. Search all Menus for a match" ,i,72);
		WriteVideo(0,5,11," ‘bAlt F7‘‘  =  List All Menus " ,i,72);

    WriteVideo(0,5,13," Labels on the right of the main window can be activated by pressing",0x7f,72);
    WriteVideo(0,5,14," the highlighted letter, for example pressing the letter ‘eT‘‘ will popup" ,0x7f,72);
    WriteVideo(0,5,15," the [Time Activated Notepad]. ",0x7f,72);


		WriteVideo(0,1,25," Press Esc to exit.....  Any key Next Page ",0x3f,80);
	  	k = GetKey();
	  	if(k == ESC)
	    	break;
	  	clrscr();

		WriteVideo(0,5, 4, " ‘bCtrl F1‘‘  = Master Password. Allows viewing of all passwords",i,72);
		WriteVideo(0,5, 5, " ‘bCtrl F2‘‘  = Print Information (No printable files)",i,72);
		WriteVideo(0,5, 6, " ‘bCtrl F3‘‘  = Credits.......We all need a little credit !",i,72);
    WriteVideo(0,5, 7, " ‘bCtrl F4‘‘  = Activate Mouse Pointer.. (Not fully installed yet !)",i,72);
    WriteVideo(0,5, 8, " ‘bCtrl F5‘‘  = De-activate Mouse Pointer",i,72);
		WriteVideo(0,5, 9, " ‘bCtrl F6‘‘  = Show Dos Window.. Try it and see",i,72);
		WriteVideo(0,5,10, " ‘bCtrl F7‘‘  = This Summary",i,72);
		WriteVideo(0,5,11," ‘bCtrl F8‘‘  = Update DOSRUN Datafiles (Every now & again)",i,72);
		WriteVideo(0,5,12," ‘bCtrl F9‘‘  = Configure Dosrun.. Colours etc...",i,72);
		WriteVideo(0,5,13," ‘bCtrl F10‘‘ = Activate Tempory Lock (If you need to have a coffee)",i,72);

		gotoxy(5,14);
		cprintf("Control Break Disabled.........");
		gotoxy(5,16);
		cprintf("If you press Ctrl + Break, you will be asked if you wish to leave ?.");
		gotoxy(5,18);
		cprintf("If system lock-up press Ctrl Break (You may have to press it twice).");

		WriteVideo(3,2,22, " [í] = This Icon appears to indicate the Menu is Password Protected ",0x1a,0);

		WriteVideo(0,1,25," Press Esc to exit.....  Any key First Page ",0x3f,80);
		k = GetKey();

		if(k == ESC)
	   	break;
		clrscr();

	}
	CloseWindow();
	RunPrompt("",0x1e);
}

//void Update(void){;}