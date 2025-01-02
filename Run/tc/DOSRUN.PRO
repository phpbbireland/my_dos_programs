void Update(void);

//void DeleteFile(char *);
void SaveScrn(void);
//void FindLine(int , int , int , int , int);
int ListWindowFile(void);
//int ScrollList(int, int, int);
void ListMenus(void);


int MenuSummary(void);
//void Scroll(void);
void GetC(void);
int CheckMouse(void);
void MatchMouse(void);

int error_win(char *);
int GetDate(void);
int GetTime(void);
int GetAllDisks(void);

void Prod(void);
void Buzzer(void);
void DosArg(int);
void UpDateMessage(void);
void RunItNow(int );
void StuffBuffer(char *);
void paint_scrn_1(void);
void SetKeys(void);
void Start(void);
void End(void);
void HookVectors(void);
void UnhookVectors(void);
void CallCritError(int i);
void ShowTimeTop(void);
void ShowDateTop(void);
void WriteRegNo(void);
void WriteBottomLine(void);
void F1function(void);
void MikeInfo(void);
void Utilities(void);
void WriteHelpRecord(int);

//void configure(void);

void LabelWindows(void);

void SaveToClip(RECORDENTRYFIELD *, int);
void RunPrompt(char *,int);
void Labels(void);
void WriteLabel(int,int,char *,int,int);
void PointTo(int, int, int);

void MouseCase(void);
void DoMouse(void);
int  Menu2Mouse(void);


int  MainLoop(RECORDENTRYFIELD *);
int  PwProt(void);

int  FindIt(char , char *);
int  YesNo(char *);

int  Mot(int); //moded july 96//
void OpenBatch(int);
void StuffKeys(char *);
void Rep(char *,int, int);
void PaintScreen1(void);
void CallNoPrint(void);
void ErrorMessage(char *);
void CheckForMenuKey(void);

static void ShowPath(int);
static int  PostMessage(char *, int);
static void GetPath(int );
static void ToLower(char *);
static void TextSearch(void);

void WriteScrnRecord(RECORDENTRYFIELD *);

void WriteTopOfMenu(RECORDENTRYFIELD *fld);

static int EditALine(RECORDENTRYFIELD *);

static void BlankARecord(RECORDENTRYFIELD *);

static void SaveRecord(void);
static void FirstRecord(void);
static void NextRecord(void);
static void LastRecord(void);
static void PrevRecord(void);

static void SeekBeginingCurRcd(int);

static void ThisRecord(void);
static void DeleteRecord(void);
static void ShowRecords(void);

void interrupt (*oldkb)(void);
void interrupt newkb(void);

static void interrupt (*oldtimer)(void);
static void interrupt newtimer(void);
static void interrupt (*oldpscreen)(void);
static void interrupt newpscreen(void);
static void interrupt (*oldctrlbrk)(void);
static void interrupt newctrlbrk(void);
static void interrupt (*oldctrlc)(void);
static void interrupt newctrlc(void);
