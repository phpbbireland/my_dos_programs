/* cursor.c */

#include <dos.h>
#include <cursors.h>
#include <interupt.h>

#define READKB 0
#define KBSTAT 1
#define SETCURSORTYPE  0x01
#define SETCURSOR      0x02
#define READCURSOR     0x03
#define READATTRCHA    0x08
#define WRITEATTRCH    0x09
#define HIDETHECURSOR  0x20
#define TALLCURSOR     0x0507 // 0107
#define SHORTCURSOR    0x0607 // 0607


static void GetCursor(void);

static int cursorpos;
static int cursorshape;


void RestoreCursor(void)
{
	_AH = SETCURSOR;
	_BH = 0;
	_DX = cursorpos;
	geninterrupt(VIDEO);
	_AH = SETCURSORTYPE;
	_CX = cursorshape;
	geninterrupt(VIDEO);
}

void NormalCursor(void)
{
	_AH = SETCURSORTYPE;
	_CX = 0x0607;
	geninterrupt(VIDEO);
}


void ChangeCursor(insmode)
{
 if(insmode)
	 SetCursor(0x0507);

 else
	 SetCursor(0x0607);

} 

void SetCursor(unsigned int shape)
{
 union REGS reg;

 reg.h.ah = 1;
 reg.x.cx = shape;
 int86(0X10, &reg, &reg);
} 

static void GetCursor(void)
{
	_AH = READCURSOR;
	_BH = 0;
	geninterrupt(VIDEO);
}

void SaveCursor(void)
{
	GetCursor();
	cursorshape = _CX;
	cursorpos   = _DX;
}

void CursorOff(void)
{
	GetCursor();
	_CH |= HIDETHECURSOR;
	_AH =  SETCURSORTYPE;
	geninterrupt(VIDEO);
}

void CursorOn(void)
{
	GetCursor();
	_CH &= ~HIDETHECURSOR;
	_AH = SETCURSORTYPE;
	geninterrupt(VIDEO);
}
