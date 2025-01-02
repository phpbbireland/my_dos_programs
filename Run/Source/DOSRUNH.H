int handle2;
int XOFF=0;
int YOFF=2;
int GlobalKey;
int GlobalLock;
int CritError;
int Leave;
int errorno;
int i = 0;
int Edit;
int GlobalCall = 0;

//static int opencount;

static int fd1;
static int fd2;
//static int fd3;

static unsigned Buffer_Full;
static unsigned LinePosition;
static unsigned HelpNo;
static unsigned Message_Waiting;
static unsigned Busy_Flag;
static unsigned DosSeg,DosBusy;

static long Record_Size;
static long Record_Count;
static long File_Length;
static long Current_Record;   //,Current_Nri;
static long Current_TRecord;  //,Current_Nri;
//static long Current_IRecord;  //


unsigned int Plength = 50;//length of displayed line for programme

unsigned long At_Page;
unsigned int  At_Line;
unsigned int  Launch_Flag;
unsigned int  Code_Flag;
unsigned int  Found_Flag;
unsigned int  WaitForKey;
unsigned int  Window_Count;
unsigned int  Mouse_Detected;
unsigned int  Window_Icon;
unsigned int  Window_Index;
unsigned int  Error_Code;

struct wininfo windowpointers[MAXWINP];
struct text_info w;
struct xwindows *win;//, *wincopy;

//*****************************************//
//               Mouse Globals             //
//*****************************************//

unsigned Mouse_X,Mouse_Y;
unsigned Mouse_Lastx, Mouse_Lasty;
unsigned Mouse_Buttons;
unsigned Mouse_Detected;

//*****************************************//
//               Menu Globals              //
//*****************************************//

int Menu_Count;

struct menuinfo menuvars[MAXMENUS];

//static int activeselect;

extern long FileIndex[30];       //store 30 locations//
extern int  FileIndexCount;      //this is the number of locations in file-1//







