                          /* Updated October 1995 */

#include <dos.h>
#include <mouses.h>

void InitMouse(void)
{ 
//	static dummy;
//	dummy++;


	_AX = 0x0000;
	geninterrupt(0x33);

//	_BX;

	if(_AX == 0)
	{
		Mouse_Detected = 0;
	}
	else
		Mouse_Detected = 1;
}

void MoveMouse(int x, int y)
{
	_AX = 0x0004;
	_CX = x; // 56;
	_DX = y; // 16;
	geninterrupt(0x33);
}

void MouseShape(int back,int chara)
{
	if(back  == 0) back  = 0xff00;
	if(chara == 0) chara = 0x0102;
	_AX = 0x000a;
	_BX = 0x0000;
	_CX = back;  //0xff00; back
	_DX = chara; //0x0102; character
	geninterrupt(0x33);
}

void MouseOn(void)//Show
{
	if(Mouse_Detected)
	{
		_AX = 0x0001;
		geninterrupt(0x33);
	}
}

void MouseOff(void)//Hide
{
 _AX = 0x0002;
 geninterrupt(0x33);
}

void GetMouseAction(void)
{
	int Mouse_Key_Count  = 0;
	if(!Mouse_Detected)
		return;

	_AX = 0x0003;
	geninterrupt(0x33);
	Mouse_Buttons = _BX;
	Mouse_X = _CX;
	Mouse_Y = _DX;

	if(Mouse_Buttons == 1)
		Mouse_Key_Count++;

	while(_BX == 1)
	{
		_AX = 0x0003;
		geninterrupt(0x33);
	}

	_AX = 0x000b;
	geninterrupt(0x33);
	if(_CX != 0 || _DX != 0)
	Mouse_Key_Count = 0;
}

void GetMouseKeys(void)
{
 _AX = 0x0005;
 _BX = 0x0000;
 geninterrupt(0x33);
 Mouse_Buttons = _AX;

// mouse_key_count = _BX;

 Mouse_X = _CX;
 Mouse_Y = _DX;
}

void WalkMouse(int from, int to, int y)
{
	int i = 0;
	if(from < to)
		for(i = from; i < to; i++)
		{
			MoveMouse(i,y);
			delay(30);
		}
	else
	for(i = from; i > to; i--)
	{
		delay(30);
		MoveMouse(i,y);
	}
}