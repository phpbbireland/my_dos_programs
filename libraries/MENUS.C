// moded 20-November-96 //
#include <alloc.h>

#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <keyios.h>
#include <cursors.h>
#include <videos.h>
#include <menus.h>
#include <xwindows.h>
#include <bios.h>
#include <interupts.h>
#include <dos.h>
#include <mouses.h>

int ftable[] =
{
 0,
 ALTA,ALTB,ALTC,ALTD,ALTE,ALTF,ALTG,ALTH,ALTI,
 ALTJ,ALTK,ALTL,ALTM,ALTN,ALTO,ALTP,ALTQ,ALTR,
 ALTS,ALTT,ALTU,ALTV,ALTW,ALTX,ALTY,ALTZ,0
};

int MaxMenus = 0;
//**************** Get sx,sy,ex,ey and fast keys for this menu *************//

void CalculateMenu(int menunumber,char *menuitems[])
{
	//new bit//
	int cnt = 0, cnt2 = 0;
	// end of new//

	int t = 0, len = 0, x = 0;
	static int sx;
  static int count;
  MaxMenus++;
//********************************* get sx *********************************//

  if(menunumber == 0) 
  { 
    menuvars[menunumber].menusx = sx = MENUSX;       // first sx = menusx
  } 
  else
  {
		sx += (strlen(menubar[count++]) + MENUBARGAP);   // calculate sx the rest
    menuvars[menunumber].menusx = sx;
  }
//**************************************************************************//

	menuvars[menunumber].mselections = menuitems;      // save pointers ??
	menuvars[menunumber].menusy = MENUSY;              // save menu sy

//****************************** get fastkeys ******************************//

	while(menuitems[x] != NULL)                        //get fastkey each line
	{
		t = strlen(menuitems[x]);
		if(t > len) 
      len = t;
		GetFastKey(menunumber,x,menuitems[x++]); 
	}
//********************************** get ey ********************************//
  menuvars[menunumber].menuey = menuvars[menunumber].menusy + x + 1; 

//********************************** get ex ********************************//

	menuvars[menunumber].menuex = menuvars[menunumber].menusx + len - MENUSX;// - 4 if needed ‘‘‘‘
	// new bit when ex for menu is more than 78//
	if(menuvars[menunumber].menuex > 77)
	{
		cnt = menuvars[menunumber].menuex;
		while(cnt-- > 78)
			cnt2++;

		menuvars[menunumber].menuex = 78;
		menuvars[menunumber].menusx = menuvars[menunumber].menusx - cnt2;

//		cprintf("Menu End Y = %d", menuvars[menunumber].menuex); GetKey();

	}// end of new bit//
//**************************************************************************//

}

void GetFastKey(int menuno, int linecount, char *menuitem)
{
	int i = 0;
	char c[2];
	for(i = 0; i < strlen(menuitem); i++)
	{
		c[0] = menuitem[i];
		if(c[0] == '‘')
		{	
			menuvars[menuno].mfastkey[linecount] = menuitem[i+2];

			textcolor(WHITE);
			return;
		}
	}
}


void MenuBar(char *menustring[]) // draw the menu bar //
{
	int i = 0, mbs = 0;
	int sx = MENUSX;
	char c[2];

	WriteVideo(0,MENUSX,MENUSY-1," ",MENUBARBC+MENUBARTC,71-MENUSX); //blank top line//

	while(menubar[i++] != NULL) mbs = i;

//cprintf("[%s]",menubar[i]); GetKey();

	for(i = 0; i < mbs; i++)
	{
		c[0] = menustring[i][1];         // allow for space first char //

		WriteVideo(0,sx,MENUSY-1,menustring[i],MENUBARBC+MENUBARTC,0);
		if(c[0] >= 'A' && c[0] <= 'Z')   // get the alt key and save it //
		{
			menuvars[i].micon   = c[0];
//			cprintf("%c",menuvars[i].micon); GetKey();

			menuvars[i].maltkey = ftable[(c[0] - 'A')+ 1];

			c[1] = '\0';
			WriteVideo(0,sx+1,MENUSY-1,c,MENUBARBC+MENUBARHC,0); //sx+1 to allow for space//

		}
		sx += (strlen(menustring[i]) + MENUBARGAP);
	}
}

int ProcessMenuKeys(int input_key)
{
	int i;
	int unknown_key = 0;
	int ret_key = 0;

	ClearBiosBuff();

	Menu_Count = 0;
//cprintf("%c",input_key); delay(1000);
	for(i = 0; i < MaxMenus; i++)            //  calculate 111
	{                                        //  from
		if(menuvars[i].maltkey == input_key)   //  table
		{                                      //  which
			Menu_Count = i;                      //  alt
			break;                               //  key
		}                                      //  was
	}                                        //  pressed
		
	if(Menu_Count < 0  || Menu_Count > MaxMenus) // trap all unwanted alts and leave
		return(0);

	do
	{
		switch(input_key)                             //
		{                                             //
			case LEFT:  if(Menu_Count == 0)             //
										Menu_Count = MaxMenus;        //
									else                            //
									Menu_Count = Menu_Count - 1;    //
									break;                          //
																									//
			case RIGHT: if(Menu_Count == MaxMenus)      //
										Menu_Count = 0;               //
									else                            //
									Menu_Count = Menu_Count + 1;    //
									break;                          //
																									//
			case ESC:   return(0);//break;              //
																									//
			default:    break;                          //
		}                                             //

		ret_key = Menu(menuvars[Menu_Count].mselections); // call menu

		if(ret_key != CR && ret_key != ESC && ret_key != LEFT && ret_key != RIGHT)
			unknown_key = 1;

		if(Menu_Count < 0)                                // roll
			Menu_Count = MaxMenus-1;                        // over

		if(Menu_Count > MaxMenus-1)                       // roll
			Menu_Count = 0;                                 // over


	}while(ret_key != ESC && ret_key != CR && unknown_key != 1);

	if(ret_key == CR)//esc
	{
		GlobalKey = ret_key;
		ret_key = MenuMatches();
		if(ret_key < 256)
			StuffKey(ret_key);
	}

	// only stuff valid keys //
	if(ret_key < 256)
		GlobalKey = ret_key;
	return(ret_key);
}

//-------------------------- internally called -----------------------------//
int Menu(char *sptr[])
{
	static int line = 0;
	int sx,ex,sy,ey;
	int key = 0;
	int i = 0;

	sx = menuvars[Menu_Count].menusx;
	sy = menuvars[Menu_Count].menusy;
	ex = menuvars[Menu_Count].menuex;
	ey = menuvars[Menu_Count].menuey;
	line = menuvars[Menu_Count].mline;

	Window_Icon = menuvars[Menu_Count].micon; //Window_Icon = 240;

	HighLightMenu();

	CursorOff();

	OpenWindow(sx,sy,ex,ey,"",0x0000,0x0b05);  //0x000b//0d         //open menu

	for(i = 0; i < ey-sy-1; i++)                                    //
		{                                                             //
			WriteVideo(0,sx+1,i+sy+1,sptr[i],MENUTEXTC,ex-sx-1);        //

			if(strstr(sptr[i],"ÄÄ"))                                  //
				WriteVideo(0,sx+1,i+sy+1,sptr[i],MENUGRAPLINE,ex-sx-1);   //
		}                                                             //

	do
	{
		if(key == UP && strstr(sptr[line],"ÄÄ"))
			line--;      // skip
		else                                                      //
		if(key == DOWN && strstr(sptr[line],"ÄÄ"))
			line++;      // skip

		menuvars[Menu_Count].mline = line;//added

		WriteVideo(1,2,line+sy,sptr[line],MENUSLINE,ex-sx-1);   // highlight

		key = GetKey();                                           //what next

		WriteVideo(1,2,line+sy,sptr[line],MENUTEXTC,ex-sx-1);   // norm

		switch(key)
		{
			case UP:	if(line == 0)
							line = ey-sy-MENUSY;
						else
							line = line -  1;
						break;

			case DOWN:	if(line == ey-sy-MENUSY)
								line = 0;
							else
								line = line + 1;
							break;

			case LEFT: 	break;

			case RIGHT:	break;

			case CR:   	break;

			case ESC:  	break;

			case 255:
							{
								if( Mouse_Y/8 < sy || Mouse_Y/8 > ey-2 || Mouse_X/8  < sx || Mouse_X/8 > ex-2)
								{ CloseWindow();return(0); }
								line = ((Mouse_Y / 8) -2);
								menuvars[Menu_Count].mline = line;
								CloseWindow();
								return(CR);
							}

			default:   	CloseWindow();
							GlobalKey = key;

							for(i = 1; i < MAXLNS; i++)
							{
								if((key) == menuvars[Menu_Count].mfastkey[i] || (key) == tolower(menuvars[Menu_Count].mfastkey[i]))
								{
									menuvars[Menu_Count].mline = i-1;
									return(CR);
								}
							}
							return(0);
				}

		menuvars[Menu_Count].mline = line;
	}while(key != CR && key != ESC && key != LEFT && key != RIGHT);
	CloseWindow();

	if(key == LEFT ) Menu_Count--;//menuvars[--Menu_Count].mline = line;
	if(key == RIGHT) Menu_Count++;//menuvars[++Menu_Count].mline = line;
	
	CursorOn();
	GlobalKey = key;
	return(GlobalKey);
}

void HighLightMenu(void) // highlight the top menu selected 
{
	int i = 0;
	int sx = MENUSX;

	MenuBar(menubar); // paint top menu

	for(i = 0; i < Menu_Count; i++)// calculate sx
		sx += (strlen(menubar[i])+MENUBARGAP);

	WriteVideo(0,sx,MENUSY-1,menubar[Menu_Count],MENUBARSC+0x0f,0);

}



int SubMenu(int sx, int sy, char *name,char *sptr[],char *sptrhelp[],int col,int type)
{
	int cnt = 0;
	int ln1 = 0, ln2 = 0, line = 0;
	int ex = 0, ey = 0;
	int zx = 3;
	int key = 0;
	int i = 0;

	CursorOff();

	while( (ln1 = strlen(sptr[cnt++])) > 1)
	{
		if(ln1 > ln2)
			ln2 = ln1;
	}

	if(line > cnt)
		line = 1;
	ex = sx + ln2 + 1;
	ey = sy + cnt + 2;

	OpenWindow(sx,sy,ex,ey,name,col,type);

	for(i = 0; i < cnt; i++)                                    //
	{                                                         //
		if(strstr(sptr[i],"ÄÄ"))                                //
			WriteVideo(zx,2,i+2,sptr[i-1],MENUGRAPLINE,0);     //

		WriteVideo(zx,2,i+2,sptr[i-1],col>>8,0);             //
	}                                                         //
	do
	{
		if(key == UP && strstr(sptr[line],"ÄÄ"))   line--;      // skip
		else                                                      //
		if(key == DOWN && strstr(sptr[line],"ÄÄ")) line++;      // skip

		WriteVideo(zx,2,line+3,sptr[line],MENUSLINE,0);   // highlight

		if(sptrhelp != NULL)
			Rep(sptrhelp[line],0x3f,10);

		key = GetKey();                                           //what next
//cprintf("%d",key); delay(2000);
		WriteVideo(zx,2,line+3,sptr[line],col>>8,0);   // norm

		switch(key)
		{
			case LEFT:
			case UP:		if(line == 0)
								line = ey-sy-4;
							else
								line = line -  1;
							break;

			case RIGHT:
			case DOWN:	if(line == ey-sy-4)
								line = 0;
							else
								line = line + 1;
							break;

			case CR:   	break;

			case PGDN:
			case PGUP:
			case HOME:
			case END:	break;
			case 255:
							{
								if( Mouse_Y/8 < sy || Mouse_Y/8 > ey-2 || Mouse_X/8  < sx || Mouse_X/8 > ex-2)
								{ CloseWindow(); key = GlobalKey = ESC; return(ESC); }
								line = ((Mouse_Y / 8) -2);
								CloseWindow();
								return(line);
							}

			default:    CloseWindow();
							GlobalKey = key;
							return(GlobalKey);
		}

	}while(key != CR && key != ESC);
	CloseWindow();

	CursorOff();
	Rep(" ",0x1f,0);
	GlobalKey = key;
	return(line);    // used to remember line and identify item //
}
