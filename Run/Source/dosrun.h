
#define ZPL 71 //path length
#define ZNL 51
#define ZDL 51

//#define MAXWINP 5

char The_Path[81];//leave this at 81 to match dosset//
char Legal_Text[] ="";
char ProgramPath[80];
char ProgramName[13];

char searchfor[50];
char searchitem[50];
char myargs[81];
char mott[10] = "CASHEL";

char dateis[12];//12 for 199?
char timeis[10];

typedef struct
{
	int fcol; int frow; int flen; int color; void  *fbuff;
} RECORDENTRYFIELD;

typedef struct
{
  int fcol; int frow; int flen; int color; int type; void *fbuff;
} TIMERECORD;

static struct config
{
  unsigned s; unsigned t; 
	int a; int b; int c;
	char qaz[20];
  char reg[60];
  int dat;
  int vgachars;
} fnd;

static struct timerecord
{
char  d1[9]; char  t1[6]; char  m1[41]; char  y1[2];
char  d2[9]; char  t2[6]; char  m2[41]; char  y2[2];
char  d3[9]; char  t3[6]; char  m3[41]; char  y3[2];
char  d4[9]; char  t4[6]; char  m4[41]; char  y4[2];
char  d5[9]; char  t5[6]; char  m5[41]; char  y5[2];
char  d6[9]; char  t6[6]; char  m6[41]; char  y6[2];
char  d7[9]; char  t7[6]; char  m7[41]; char  y7[2];
char  d8[9]; char  t8[6]; char  m8[41]; char  y8[2];
char  d9[9]; char  t9[6]; char  m9[41]; char  y9[2];
} tnd;

static struct fileinfo
{
char  menuname[ZNL]; 
char  mpmot[21];
char  item1[ZDL];
char  item2[ZDL];
char  item3[ZDL];
char  item4[ZDL];
char  item5[ZDL];
char  item6[ZDL];
char  item7[ZDL];
char  item8[ZDL];
char  item9[ZDL];
char  item10[ZDL];
char  pathmot[10][ZPL];
char  ppmot[10][11];
} nd;

static TIMERECORD trec[] =
{
{  5, 5, 8, 0x0f, 2, tnd.d1 },
{ 16, 5, 5, 0x0f, 2, tnd.t1 },
{ 24, 5,40, 0x0f, 0, tnd.m1 },
{ 67, 5, 1, 0x0f, 0, tnd.y1 },
{  5, 6, 8, 0x0f, 2, tnd.d2 },
{ 16, 6, 5, 0x0f, 2, tnd.t2 },
{ 24, 6,40, 0x0f, 0, tnd.m2 },
{ 67, 6, 1, 0x0f, 0, tnd.y2 },
{  5, 7, 8, 0x0f, 2, tnd.d3 },
{ 16, 7, 5, 0x0f, 2, tnd.t3 },
{ 24, 7,40, 0x0f, 0, tnd.m3 },
{ 67, 7, 1, 0x0f, 0, tnd.y3 },
{  5, 8, 8, 0x0f, 2, tnd.d4 },
{ 16, 8, 5, 0x0f, 2, tnd.t4 },
{ 24, 8,40, 0x0f, 0, tnd.m4 },
{ 67, 8, 1, 0x0f, 0, tnd.y4 },
{  5, 9, 8, 0x0f, 2, tnd.d5 },
{ 16, 9, 5, 0x0f, 2, tnd.t5 },
{ 24, 9,40, 0x0f, 0, tnd.m5 },
{ 67, 9, 1, 0x0f, 0, tnd.y5 },
{  5,10, 8, 0x0f, 2, tnd.d6 },
{ 16,10, 5, 0x0f, 2, tnd.t6 },
{ 24,10,40, 0x0f, 0, tnd.m6 },
{ 67,10, 1, 0x0f, 0, tnd.y6 },
{  5,11, 8, 0x0f, 2, tnd.d7 },
{ 16,11, 5, 0x0f, 2, tnd.t7 },
{ 24,11,40, 0x0f, 0, tnd.m7 },
{ 67,11, 1, 0x0f, 0, tnd.y7 },
{  5,12, 8, 0x0f, 2, tnd.d8 },
{ 16,12, 5, 0x0f, 2, tnd.t8 },
{ 24,12,40, 0x0f, 0, tnd.m8 },
{ 67,12, 1, 0x0f, 0, tnd.y8 },
{  5,13, 8, 0x0f, 2, tnd.d9 },
{ 16,13, 5, 0x0f, 2, tnd.t9 },
{ 24,13,40, 0x0f, 0, tnd.m9 },
{ 67,13, 1, 0x0f, 0, tnd.y9 },
{  0, 0, 0, 0,    0, NULL   }
};

static RECORDENTRYFIELD Per_Entry_Field[] =
{
{ 1,  0, 50, 0x1f, nd.menuname }, //1b  //1 67
{ 1,  0, 20, 0x1f, nd.mpmot },
{ 5,  1, 50, 0x1f, nd.item1 },
{ 5,  2, 50, 0x1f, nd.item2 },
{ 5,  3, 50, 0x1f, nd.item3 },
{ 5,  4, 50, 0x1f, nd.item4 },
{ 5,  5, 50, 0x1f, nd.item5 },
{ 5,  6, 50, 0x1f, nd.item6 },
{ 5,  7, 50, 0x1f, nd.item7 },
{ 5,  8, 50, 0x1f, nd.item8 },
{ 5,  9, 50, 0x1f, nd.item9 },
{ 5, 10, 50, 0x1f, nd.item10},
{ 0,  0,  0,    0, NULL     }
};

static struct ClipBoard
{
char  ClipItemA[ZDL];
char  ClipPathmotA[ZPL];
char  ClipItemB[ZDL];
char  ClipPathmotB[ZPL];
}ClipLines;

//static char QUESTION []= "To run a DOS command you need only type a ‘a?‘‘ and press Enter.";
static char Launching  []= " Launching.......";
static char TOPLINE    []= " ‘bD‘‘osRun / The Dos ‘bN‘‘avigator (C) Michael O'Toole (1984-1996)  ";
static char enterdata  []= "\( Enter Description \) Press Enter to Save, Esc to Ignore";
static char menuprotect[]= "Menu protected by Password... Remove protection first.. ";
static char movebuffer []= "File Saved to Buffer: Move to new position and press F6 ";
static char doubleline []= "ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ";
static char T1         []= "‘aÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿";
static char T2         []= "‘a³   Date      Time   Messages..........................         û ³";
static char T3         []= "‘aÃÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄ´";
static char T4         []= "‘a³          ³       ³                                          ³   ³";
static char TD         []= "‘aÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÙ";


