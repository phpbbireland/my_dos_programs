#include <dos.h>
#include <mmouse.h>

unsigned mousex_position,mousey_position;
unsigned mouse_button_status,mouse_key_count;
unsigned mouse_lasty, mouse_lastx;


int mouseinit(void)
{ 
	int prs;              // prs = present
	_AX = 0x0000;
	geninterrupt(0x33);
	prs = _AX;

	if(prs == 0)
		return(0);
	else
	return(1);
}
void mousegotoxy(int x, int y)
{
	_AX = 0x0004;
	_CX = x; // 56;
	_DX = y; // 16;
	geninterrupt(0x33);
}

void mouseshape(int back,int chara)
{
	if(back  == 0) back  = 0xff00;
	if(chara == 0) chara = 0x0102;
	_AX = 0x000a;
	_BX = 0x0000;
	_CX = back;  //0xff00; back
	_DX = chara; //0x0102; character
	geninterrupt(0x33);
}

void mouseon(void)
{
	_AX = 0x0001;
	geninterrupt(0x33);
}

void mouseoff(void)
{
 _AX = 0x0002;
 geninterrupt(0x33);
}

void getmouse(void)
{
	_AX = 0x0003;
	geninterrupt(0x33);
	mouse_button_status = _BX;
	mousex_position = _CX;
	mousey_position = _DX;

	if(mouse_button_status == 1)
		mouse_key_count++;

	while(_BX == 1)
	{
		_AX = 0x0003;
		geninterrupt(0x33);
	}

	_AX = 0x000b;
	geninterrupt(0x33);
	if(_CX != 0 || _DX != 0)
	mouse_key_count = 0;
}

void mousekeyclr(void)
{
 _AX = 0x0005;
 _BX = 0x0000;
 geninterrupt(0x33);
 mouse_button_status = _AX;
// mouse_key_count = _BX;
 mousex_position = _CX;
 mousey_position = _DX;
}

void showmouse(void)
{
	_AX = 0x0003;
	geninterrupt(0x33);
	mousex_position = _CX;
	mousey_position = _DX;

 _AX = 0x0004;
 _CX = mousex_position;
 _DX = mousey_position;
 geninterrupt(0x33);
}

/*
void main(void)
{
	int i = 0;
	mouseinit();
	mouseon();
	while(mouse_button_status != 2)
	{
		getmouse();
		if(mouse_button_status == 1)
		gotoxy(10,10);
		cprintf("          \r [%d] [%d] [%d]\r",mousex_position,mousey_position,mouse_key_count);
	}
	mouseoff();
}
*/