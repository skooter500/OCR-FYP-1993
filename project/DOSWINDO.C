/***************************************************************************
 *                                                                         *
 *                 DOS Window drawing functions                            *
 *                             by                                          *
 *             Richard Brennan & Bryan Duggan (c) 1993                     *
 *                                                                         *
 ***************************************************************************/




/* box_type :    0 = no frame,  1 = single line, 2 = double line */
/* shadow type : 0 = no shadow, 1 = opaque,      2 = transparent */



/*-----[ Global Variables ]-----*/

extern int shadow_color;
extern int warning_color_f, warning_color_w;
extern int message_color_f, message_color_w;
extern int inverse_color;
unsigned char far *vram;
struct text_info screen;
char gfilename[30];
char gdirectory[30];
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




struct text_info wkw;  /* working window */
struct text_info windows[ MAX_WINDOWS ];

struct {
  int frame, shadow;
  char *wsave;
} dressing[ MAX_WINDOWS ];

static int curr_wnd = 1;  /*---[ current window ]---*/
static char shadow_char = 176;

int shadow_color = 7;
int warning_color_f = 205, warning_color_w = 78;
int message_color_f =  19, message_color_w = 19;
int inverse_color = 112;

/*=====================================================
  CHANGES THE SCREEN ATTRIBUTES BETWEEN TWO X COORDINATES
  ON ONE LINE TO HIGHLIGHT ANYTHING THERE
  =======================================================*/

void highlight_on(int width)
{   int x,y,p;
    unsigned char far *pointer;
    gettextinfo(&screen);
    y = wherey();
    x = wherex();

    for( p=x; p<(x+width); p++ )
         {
          pointer=(vram + ( screen.winleft + p - 2 ) * 2
        + ( screen.wintop + y - 2 ) * 160 + 1 );

          if ((*pointer==TEXTCOLOUR)||(*pointer==MESSAGECOLOUR))
            *pointer=HIGHCOLOUR;
          else
          if (*pointer==ALTCOLOUR)
             *pointer=HIGHCOLOUR1;
         }
}


/*===========================================================
   RESTORES THE SCREEN ATTRIBUTES BETWEEN TWO X COORDINATES
   ON ONE LINE TO DEHIGHLIGHT THE LINE
   ====================================================*/

void highlight_off(int width)
{   int x,y,p;
    unsigned char far *pointer;
    gettextinfo(&screen);
    y = wherey();
    x = wherex();

    for( p=x; p<(x+width); p++ )
         {
          pointer=(vram + ( screen.winleft + p - 2 ) * 2
        + ( screen.wintop + y - 2 ) * 160 + 1 );

          if (*pointer==HIGHCOLOUR)
            *pointer=TEXTCOLOUR;
          else
          if (*pointer==HIGHCOLOUR1)
             *pointer=ALTCOLOUR;
         }
}



void mylight_on( int hcolor, int length )
{ int p,x,y;
  x=wherex();
  y=wherey();
  for (p=x;p<(x+length);p++) {
     *(vram + (screen.winleft + p - 2 ) * 2
     +(screen.wintop + y - 2 ) * 160 + 1 )
     =hcolor;
  }
}
void mylight_off( int length )
{ int p,x,y;
  x=wherex();
  y=wherey();
    for (p=x;p<(x+length);p++) {
     *(vram + (screen.winleft + p - 2 ) * 2
     +(screen.wintop + y - 2 ) * 160 + 1 )
     =screen.attribute;
  }
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   + Write a String to the Screen                          +
   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void string_out( char *string )
{
    int x,y;
    x = wherex();
    y = wherey();
    while( *string ) {
        if( *string == '\t' ) {
            do ; while( ++x % 8 != 1 );
            string++;
            continue;
        }
        *(vram + ( screen.winleft + x++ -2 ) * 2
          + ( screen.wintop + y - 2 ) * 160 ) = *string++;
        if( x > screen.winright - screen.winleft + 1 ) break;
    }
    gotoxy( x,y );
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   + Overlay One String With Another                       +
   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void overlay( char *string )
{
    int x,y;
    x = wherex();
    y = wherey();
    while (*string) {
        if( *string != ' ' ) *(vram + ( screen.winleft + x - 2 )
          * 2 + ( screen.wintop + y - 2 ) * 160 ) = *string;
        if( *string != ' ' ) *(vram + ( screen.winleft + x - 2 )
          * 2 + ( screen.wintop + y - 2 ) * 160 + 1 )
          = screen.attribute;
        x++; string++;
        if ( x > screen.winright - screen.winleft + 1 ) break;
    }
    gotoxy( x, y );
}



void gputxy(int x,int y,int attrib,char *string)
{

   int p,width;
   width=strlen(string);
   gettextinfo(&screen);
   for( p=x; p<(x+width); p++ )
    {
        *(vram + (  + p - 2 ) * 2
        + (  + y - 2 ) * 160+1)
        =attrib;
        *(vram + (  + p - 2 ) * 2
        + (  + y - 2 ) * 160)
        =string[p-x];
    }
}



int open_window( int left, int top, int right, int bottom,
                 int box_type, int shadow_type,
                 int f_color, int w_color )
{
  int frame, shadow, bufsize, width, height, k, index;

  shadow = ( shadow_type ) ? 1 : 0;
  frame  = ( box_type ) ? 1 : 0;

  if ( frame )
  {
     --left;
     --top;
     ++right;
     ++bottom;
  }
  /*-----[ check that window is within screen boundary ]-----*/
  if( left < 1 || right+shadow > 80  || right < left) return (0);
  if( top < 1  || bottom+shadow > 25 || bottom < top) return (0);

  width = right-left+1;
  height = bottom-top+1;

  if( curr_wnd < MAX_WINDOWS )
  {
     if( curr_wnd == 1 ) gettextinfo(windows);
     else
     {
        windows[curr_wnd-1].curx = wherex();
        windows[curr_wnd-1].cury = wherey();
     }

     /*-----[ allocate buffer and save contents of window ]-----*/
     bufsize = (bottom-top+1+shadow) * (right-left+1+shadow) * 2;
	 if ((dressing[curr_wnd].wsave =(char *) malloc(bufsize)) == NULL ){
        clrscr();
        gotoxy(10,10);
        cputs("FATAL ERROR,MEMORY OVERFLOW");
        abort();
        return(0);
       }
     gettext( left,top,right+shadow,bottom+shadow,
              dressing[curr_wnd].wsave);

     if( box_type )
       box( left,top,right,bottom,box_type,f_color);

     if( shadow_type )
     {
        textattr( shadow_color );
        window(1,1,80,25);
        for( k=1; k <= height; k++ )  /*---[ draw vertical line ]---*/
        {
            gotoxy( right+1, top+k );
            index = ((width+1) * (k+1) -1)*2;
            putch( shadow_type == 1 ? shadow_char :
                   *(dressing[curr_wnd].wsave + index ));
        }
        gotoxy( left+2, bottom+1 );  /*---[ draw horizontal line ]---*/
        for( k=2; k<width; k++ )
        {
            index = ((width+1) * height +k)*2;
            putch( shadow_type == 1 ? shadow_char :
                   *(dressing[curr_wnd].wsave + index ));
        }
     }

     /*-----[ Set window to user requirements ]-----*/
     textattr( w_color );
     window( left, top, right, bottom );
     gettextinfo(&wkw); /*---[ remember window details ]---*/
     dressing[curr_wnd].frame = box_type;
     dressing[curr_wnd].shadow = shadow_type;
     if( frame )
       window( ++left,++top,--right,--bottom );
     windows[curr_wnd++] = wkw;
     clrscr();
  }
  return(1);
}


/*-----[ close window ]-----*/
int close_window( void )
{
    int shadow=0;

    /*---[ Get allocated buffer and restore contents of window ]---*/
    if( curr_wnd > 1 )
    {
        if( dressing[curr_wnd-1].shadow ) shadow = 1;

    /*---[ restore window text ]---*/
        if( ! (puttext( wkw.winleft,wkw.wintop,wkw.winright+shadow,
                     wkw.winbottom+shadow,dressing[curr_wnd-1].wsave)))
          return(0);

        free( dressing[curr_wnd-1].wsave); /*---[ free memory buffer ]---*/
        wkw = windows[(--curr_wnd)-1];
        textattr(wkw.attribute);
        if ( dressing[curr_wnd-1].frame )
            window(wkw.winleft+1,wkw.wintop+1,wkw.winright-1,
            wkw.winbottom-1);
        else
            window( wkw.winleft,wkw.wintop,wkw.winright,wkw.winbottom);

        gotoxy(wkw.curx,wkw.cury); /*---[ reset cursor ]---*/

    }
    return(1);
}



/*-----[ BOX ]-----*/
/* box_type - 0 no lines,  - 1 single line,  - 2 double line */

int box        ( int left, int top, int right, int bottom,
                 int box_type,int f_color )
{
    int k, hlength, vlength;
    char nw,ne,se,sw,hline,vline;
    struct text_info t_info;

    if( left<1 || top<1 || right>80 || bottom>25 ||
        right<left || bottom<top )
      return(0);

    if ( !box_type )
      nw = ne = se = sw = vline = hline = ' ';
    else
    {
      /*---[ window corners ]---*/
      nw     = ( box_type == 1 ) ? '\xda' : '\xc9';
      ne     = ( box_type == 1 ) ? '\xbf' : '\xbb';
      se     = ( box_type == 1 ) ? '\xd9' : '\xbc';
      sw     = ( box_type == 1 ) ? '\xc0' : '\xc8';
      vline  = ( box_type == 1 ) ? '\xb3' : '\xba';
      hline  = ( box_type == 1 ) ? '\xc4' : '\xcd';
    }
    gettextinfo( &t_info );
    textattr( f_color );
    window( 1,1,80,25 );
    hlength = right - left - 1;
    vlength = bottom - top;

    /*---[ top of box ]---*/
    gotoxy( left, top );
    putch( nw );
    for( k=0; k < hlength; k++ )
      putch( hline );
    putch( ne );

    /*---[ sides of box ]---*/
    for( k=1; k < vlength; k++ )
    {
      gotoxy( left, top+k );
      putch( vline );
      gotoxy( right, top+k );
      putch( vline );
    }

    /*---[ bottom of box ]---*/
    gotoxy( left,bottom );
    putch( sw );
    for( k=0; k < hlength; k++ )
      putch( hline );
    putch( se );

    /*---[ restore screen ]---*/
    window( t_info.winleft, t_info.wintop,
            t_info.winright,t_info.winbottom );

    gotoxy( t_info.curx,t_info.cury );
    textattr( t_info.attribute );
    return(1);
}



/*****************************************************************
 *                                                               *
 *                    Pre-defined Windows.                       *
 *                                                               *
 *****************************************************************/

void pause(void)
{
 getch();
}


void bottombar(char *statusmsg)

{

  open_window(1,25,80,25,0,0,0,TEXTCOLOUR2);
  clrscr();
  gotoxy( 40-(strlen(statusmsg)/2) ,1);
  cputs(statusmsg);
}

void topbar(char *statusmsg)

{

  open_window(1,1,80,1,0,0,0,TEXTCOLOUR2);
  clrscr();
  gotoxy( 40-(strlen(statusmsg)/2) ,1);
  cputs(statusmsg);
}


void writeerror(int x,int y,char *errmsg)
{
 int xpos,width;
/* xpos=36-(strlen(errmsg)/2);*/
 width=strlen(errmsg)+5;
 open_window(x,y,x+width,y,2,2,ERRORCOLOUR1,ERRORCOLOUR);
 gotoxy(4,1);
 cputs(errmsg);
 while((getch())!=ESC);
 close_window();
}



void bottomwindow(char *message)
{
 int xpos,width;
 xpos=36-(strlen(message)/2);
 width=strlen(message)+5;
 open_window(xpos,21,xpos+width,21,2,2,MESSAGECOLOUR,MESSAGECOLOUR);
 gotoxy(4,1);
 puts(message);
}



void putwindow(int y,int depth,char *prompt)
{
 int width=strlen(prompt),xpos;

 xpos=39-((width+4)/2);


 open_window(xpos,y,xpos+width+4,y+depth-1,2,2,MESSAGECOLOUR,MESSAGECOLOUR);
 gotoxy(3,1);
 cputs(prompt);

}


int getmenubar(int startoption,char *options)
{
 int i,j=1;
 int step,rounderror;
 int numoptions=0;
 int xpos=1,ypos=1;
 int option=1,key;


 /* establish number of options    */
 for (i=0;i<=strlen(options);i++)
        if (options[i]=='\n')
           numoptions++;
 step=(int) (79/(numoptions));

 rounderror=((79-(step*numoptions))/2);

 j=rounderror;
 open_window(2,2,79,2,1,0,TEXTCOLOUR,TEXTCOLOUR);


 for (i=1;i<strlen(options);i++)
     {
      gotoxy(j,1);
      if (options[i]=='\n')
         {
          option++;
          j=(step*option)-(step)+rounderror;
         }
      else
          {
           gotoxy(j,1);
           putch(options[i]);
           j++;
          }
     }
 option=startoption;
 do
       {
        xpos=(option*(step))-(step)+rounderror;
        gotoxy(xpos,ypos);
        highlight_on(step);
        key=getch();
        if (key==0)
         {
           switch(key=getch())
            {
             case RIGHT:{highlight_off(step);
                         if (option==(numoptions))
                            option=0;
                         xpos=((step)*++option)-(step)+rounderror;
                         gotoxy(xpos,ypos);
                         highlight_on(step);
                         break;}
             case LEFT: {highlight_off(step);
                         if (option==1)
                            option=(numoptions+1);
                         xpos=((step)*--option)-(step)+rounderror;
                         gotoxy(xpos,ypos);
                         highlight_on(step);
                         break;}
            }
            xpos=((step)*option)-(step)+rounderror;
         gotoxy(xpos,ypos);
         }

       }
       while((key!=ENTER)&&(key!=ESC));

 if (key!=ENTER)
    return(ESC);
 else
     return(option);

}

int getdropdown(int x,int y,int start, char *options)
{
 int xpos=1,ypos=1,width,maxwidth=0,depth=0,option=start,i,key,attribflag=FALSE;

 /* establish width and depth of window */
for (i=0;i<strlen(options);i++)
    {
        if (options[i]=='\n')
           {
            depth++;
            width=0;
           }
        else
            {
             if (options[i]!='*')
                {width++;}
             if (width>maxwidth)
                maxwidth=width;
            }
        }

    open_window(x,y,x+(maxwidth),y+(depth-1),1,2,BORDERCOLOUR,TEXTCOLOUR);

    for (i=1;i<strlen(options);i++)
     {

      gotoxy(xpos,ypos);
      if (options[i]=='\n')
         {
          ypos++;
          xpos=1;
         }
      else
          {
           gotoxy(xpos,ypos);
           if (options[i]!='*')
              {putch(options[i]);xpos++;}
           else
               {
                if (attribflag==FALSE)
                  {textattr(ALTCOLOUR);attribflag=TRUE;}
                else
                if (attribflag==TRUE)
                  {textattr(TEXTCOLOUR);attribflag=FALSE;}
               }
          }
     }
     xpos=ypos=1;




 do
       {
       ypos=option;
       gotoxy(xpos,ypos);
       highlight_on(maxwidth+1);
        key=getch();
        if (key==0)
         {
           switch(key=getch())
            {
             case UP:{   highlight_off(maxwidth+1);
                         if (option==1)
                            option=(depth+1);
                         ypos=(--option);
                         gotoxy(xpos,ypos);
                         highlight_on(maxwidth+1);
                         break;
                      }
             case DOWN: {
                         highlight_off(maxwidth+1);
                         if (option==(depth))
                            option=0;
                         ypos=(++option);
                         gotoxy(xpos,ypos);
                         highlight_on(maxwidth+1);
                         break;
                        }
            }
         ypos=(option);
         gotoxy(xpos,ypos);
         }

       }
       while((key!=ENTER)&&(key!=ESC));
 if (key!=ENTER)
 {
    return(ESC);
 }
 else
     return(option);
}


/*              ******************************************
   THIS IS A ONE LINE EDITOR WHICH READS A STRING 'LINE' ALLOWING
   UP TO 'LENGTH CHARACTERS TO BE PASSED INTO IT
   IT BEGINS AT THE CURRENT CURSOR POSITION WHICH IT SAVES AS X1,Y1 */

int readline(char *line,int length)
{  char ch=' ';
   int i=0;
   int x,y,start;
   x=wherex();
   y=wherey();
   highlight_on(length);
   while ((ch!=ESC) && (ch!=ENTER) && (i<=length))
   {  ch=getch();
      ch=toupper(ch);
      if (((ch>='A') && (ch<='Z')) || ((ch>='0')&&(ch<='9')) ||
          (ch=='\\' ) || (ch==':') )
         { line[i]=ch;
           i++;              /* IF A GOOD CHARACTER ECHO IT */
           putc(ch,stdout);
          }
       if ((ch==LEFT) || (ch==BACKSPACE))
       {  x=wherex();
          if (i!=0)          /* BACKSPACE OR LEFT CAUSE IT TO DELETE THE */
          { line[i]='\0';    /* LAST CHARACTER OFF SCREEN AND REMOVE IT */
            i--;             /* FROM THE STRING */
            x--;
            puts("\b ");
            gotoxy(x,y);
           }
        }

        line[i]='\0';
     }
     gotoxy(1,wherey());
     if (ch==ESC)
         return(0);    /* IF ESCAPE WAS PRESSED IT RETURNS A FALSE */
     else
         return(1);

}


 /**********************************************************************
 *                                                                    *
 *  SAME AS READ LINE, BUT THIS FUNCTION READS DIGITS NOT CHARACTERS  *
 *    RETURNS TRUE IF THE DIGITS WERE VALID AND FALSE IF NOT.         *
 *                                                                    *
 **********************************************************************/




int readnum(char *line,int length,int max)
{  char ch=' ';
   int i=0,amount=0,again=TRUE;
   int x,y,x1,y1,start,j;
   x=wherex();
   y=wherey();
   x1=x;
   y1=y;
   highlight_on(length);
   do
   {  ch=' ';
      while ((ch!=ESC) && (ch!=ENTER) && (i<=length))
      { ch=getch();
        ch=toupper(ch);
        if ((ch>='0')&&(ch<='9'))
           { line[i]=ch;  /* IF ITS A DIGIT ECHOE IT */
             i++;
             putc(ch,stdout);
           }
        if ((ch==LEFT) || (ch==BACKSPACE))
        {  x=wherex();
           if (i!=0)
           { line[i]='\0';
             i--;          /* DELETE LAST ONE */
             x--;
             puts("\b ");
             gotoxy(x,y);
            }
         }
         if (ch==ESC)
            again=FALSE;
         if ((i>0)&&(ch==ENTER)) /* IF NOT AN EMPTY STRING EVALUATE IT */
         {
           line[i]='\0';
           amount=(atoi(line));
           if (amount<=max)    /* IF ITS IN RANGE YOU ARE FINISHED */
               again=FALSE;    /* ANOTHER DIGIT WILL INCREASE IT */
           else
              { ch=' ';
                x=x1;          /* IN ORDER TO READ AGAIN THE EDITOR MUST */
                i=0;           /* CLEAR THE LINE 'LENGTH' CHARACTERS LONG*/
                y=y1;          /* FROM X1,Y1 AND HIGHLIGHT IT */
                gotoxy(x,y);
                j=0;
                while (j<=length)
                {  putc(' ',stdout);
                   j++;
                   gotoxy(j+x1,y1);

                }
                gotoxy(x1,y1);
                highlight_on(length);
              }
         }
      }
  }
  while (again==TRUE);
     highlight_off(length);
     if (ch==ESC)
         return(0);  /* THE RETURN INTEGER IS NOT THE VALUE READ BUT */
     else            /* WHETHER OR NOT ESCAPE WAS PRESSED */
         return(1);

}



void doswindows(void)
{
 gettextinfo(&screen);
 switch(screen.currmode){
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
	case 6: vram=(unsigned char *) MK_FP(0xB800,0);break;
	case 7: vram=(unsigned char *)MK_FP(0xB000,0);break;
	case 8: vram=(unsigned char *)MK_FP(0xA000,0);break;}
	textbackground(BLUE);
	clrscr();
 }
