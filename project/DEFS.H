#define TEXTCOLOUR BLACK+(LIGHTGRAY<<4)
#define ALTCOLOUR RED+(LIGHTGRAY<<4)
#define BORDERCOLOUR BLACK+(LIGHTGRAY<<4)
#define MYCOLOUR RED + (CYAN<<4)
#define TEXTCOLOUR2 BLACK+(LIGHTGRAY<<4)
#define HIGHCOLOUR BLACK+(GREEN<<4)
#define HIGHCOLOUR1 RED +(GREEN<<4)
#define ERRORCOLOUR RED+(CYAN<<4)+BLINK
#define ERRORCOLOUR1 RED+(CYAN<<4)
#define MESSAGECOLOUR WHITE+(GREEN<<4)
#define BACKCOLOUR BLUE
#define MAX_WINDOWS 10
#define TRUE 1
#define FALSE 0
#define BORDER2 1
#define ASKCOLOR 8
#define HIGHCOLOR 2
#define ORDCOLOR 8
#define LEFTKEY 53
#define RIGHTKEY 55
#define CR 13
#define UPKEY 57
#define DOWNKEY 51
#define HIGHLIGHT 4
#define NOHIGHLIGHT 5
#define UP 72
#define RIGHT 77
#define DOWN 80
#define LEFT 75
#define ENTER 13
#define ESC 27
#define BACKSPACE 8
#define END  79
#define PGDN 81
#define PGUP 73
#define NOFRAME 0
#define SINGLELINE 1
#define DOUBLELINE 2
#define NOSHADOW 0
#define OPAQUE 1
#define TRANSPARENT 2


/*-----[ Function Declarations ]-----*/

int open_window( int left, int top, int right, int bottom,
                 int box_type, int shadow_type,
                 int f_color, int w_color );
int box        ( int left, int top, int right, int bottom,
                 int box_type,int f_color );
void highlight_on(int width);
void highlight_off(int width);
int close_window( void );
void mylight_on( int hcolor, int length );
void mylight_off( int length );
void string_out( char *string );
void overlay( char *string );
void pause(void);
void topbar(char *statusmsg);
void bottombar(char *statusmsg);
void writeerror(int x,int y,char *errmsg);
void bottomwindow(char *message);
void putwindow(int y,int depth,char *prompt);
int getmenubar(int startoption,char *options);
int getdropdown(int x,int y,int start,char *options);
int readline(char *line,int length);
int readnum(char *line,int length,int max);
void doswindows(void);
void gputxy(int x,int y,int attrib,char *string);