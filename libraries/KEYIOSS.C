#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <keyios.h>
#include <interupt.h>

static void interrupt (*oldkb)(void); // interrupt function before call //

int kbxlat[] = {
FIVE,       0,76,
CTRLUP,     4,72,
CTRLFIVE,   4,76,
CTRLDOWN,   4,80,
CTRLINS,    4,82,
CTRLDEL,    4,83,
ALTHOME,    8,71,
ALTUP,      8,72,
ALTPGUP,    8,73,
ALTBS,      8,75,
ALTFIVE,    8,76,
ALTFWD,     8,77,
ALTEND,     8,79,
ALTDN,      8,80,
ALTPGDN,    8,81,
ALTINS,     8,82,
ALTDEL,     8,83,
0
};

        /*  interrupt service routine  to translate newkeys  */

static void interrupt ExtendedKeys()
{
	static int scancode, shiftkeys, *xl;
	scancode  = inportb(KBDATA);
	shiftkeys = peekb(0x40,0x17);
	for(xl = kbxlat; *xl; xl += 3)
	if(scancode == *(xl+2))
	  if((shiftkeys & 0x2f) == *(xl + 1))
      StuffKey(*xl);
      	(*oldkb)();
}


/* example call to return extended keys */
/*
void main(void)
{

  int c;
  oldkb = getvect(KYBRD);
  setvect(KYBRD, ExtendedKeys);

  while((c = Getkey()) != ESC)
    cprintf("%d %c",c,isprint(c) ? C : '.');

 setvect(KYBRD, oldkb);
}
*/
