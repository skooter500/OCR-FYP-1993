// APPLICATION FILE

#include <graphics.h>
#include <stdio.h>
#include <alloc.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <ctype.h>
#include "defs.h"
//#include "doswindow.c"
#include "bitmap.h"  // INTERFACE & IMPLIMENTATION FILES
//#include "bitmap.cpp"



byte screensave[4096];
void graphicsmode(void)
	{	/* request auto detection */
		int gdriver=VGA,gmode=VGAHI, errorcode;

                /* register a driver that was added into graphics.lib */
                /* For information on adding the driver, see the
                /* BGIOBJ section of UTIL.DOC */
                errorcode = registerbgidriver(EGAVGA_driver);

                /* report any registration errors */
                if (errorcode < 0)
                {
                   cprintf("Graphics error: %s\n", grapherrormsg(errorcode));
                   cprintf("Press any key to halt:");
                   getch();
                   exit(1); /* terminate with an error code */
                }
		/* initialize graphics and local variables */


		/* read result of initialization */
                initgraph(&gdriver, &gmode, "");
		errorcode = graphresult();
		if (errorcode != grOk)  /* an error occured */
			{	cprintf("Graphics error: %s\r\n", grapherrormsg(errorcode));
				cprintf("Press any key to halt:");
				getch();
				exit(1); /* terminate with an error code */
			}
		settextjustify(CENTER_TEXT, CENTER_TEXT);
		settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
		setbkcolor(BLUE);
	}




// THE NEXT THREE FUNCTIONS WILL PLAY THE OUTPUT OF THE PROGRAM
// THROUGH THE PC'S INTERNAL SPEAKER. THIS FEATURE IS FOR
// IS PURELY INTENDED TO DEMONSTRATE A PRACTICAL APPLICATION OF THE
// SYSTEM
void tone(int freq, int dur)
{
	sound(freq);
	delay(dur);
	nosound();
}

int findpitch(char *str)
	{  if (strcmp(str,"[LOW D")==0)
		  return(LOWD);
	   if (strcmp(str,"[LOW E")==0)
		  return(LOWE);
		  if (strcmp(str,"[LOW F")==0)
		  return(LOWF);
	   if (strcmp(str,"[LOW G")==0)
		  return(LOWG);
	   if (strcmp(str,"[LOW A")==0)
		  return(LOWA);
	   if (strcmp(str,"[LOW B")==0)
		  return(LOWB);
	   if (strcmp(str,"[HI C]")==0)
		  return(HIC);
	   if (strcmp(str,"[HI D]")==0)
		  return(HID);
	   if (strcmp(str,"[HI E]")==0)
		  return(HIE);
	   if (strcmp(str,"[HI F]")==0)
		  return(HIF);
	   if (strcmp(str,"[HI G]")==0)
		  return(HIG);
	   return(DEFAULT);
}


void play(char * mus)
	{	int i=0,len=0,curptr=0;
		char current[10];
		len=strlen(mus);
		nosound();
		while(i<len)
			{  if ((mus[i]!=' ')&&(mus[i]!='-')&&(mus[i]!='\r')&&(mus[i]!='\n'))
				{	curptr=0;
					while((mus[i]!=' ')&&(mus[i]!='-')&&(mus[i]!='\r')&&(mus[i]!='\n'))
						 current[curptr++]=mus[i++];
					current[curptr]='\0';
					if (strcmp(current,"CROCHET")==0)
						{  i+=3;
						   for (curptr=0;curptr<=6;curptr++)
							current[curptr]=mus[i++];
						   current[--curptr]='\0';
						   tone(findpitch(current),CROCHET);
						}
					if (strcmp(current,"MINUM")==0)
						{   i+=3;
							for (curptr=0;curptr<=6;curptr++)
								current[curptr]=mus[i++];
							current[--curptr]='\0';
							tone(findpitch(current),MINUM);
						}
					if (strcmp(current,"DOUBLE")==0)
						{  	i+=9;
							for (curptr=0;curptr<=6;curptr++)
								current[curptr]=mus[i++];
							current[--curptr]='\0';
							tone(findpitch(current),QUAVER);
							while ((mus[i]==' ')||(mus[i]=='-')||(mus[i]=='\r')||(mus[i]=='\n'))
								{i++;}
							for (curptr=0;curptr<=6;curptr++)
							current[curptr]=mus[i++];
							current[--curptr]='\0';
							tone(findpitch(current),QUAVER);
						}

				}
			i++;
			}
		}  // END FUNCTION


void setupscreen(void)
	 {    topbar("Musical OCR System (c) 1993 Bryan Duggan, DIT. Kevin St.");
		  bottombar("Select an option");
	 }


int main(void)
 {  int input,grflag,coflag=1,flag;   // GRAPHICS/COMMENTS FLAGS
	page sample(80,80,"");  // PAGE DATA
	segment section;        // CURRENTLY ANALYSED SECTION
	hmap horz;              // HORIZONTAL DENSITY MAP
	vmap vert;              // VERTICAL MAP
	stavepositions spos;    // POSITIONS OF THE STAVES
	possibles notes;        // POTENTIAL NOTE/BAR/CLEF POSITIONS
	int j;                  // COUNTER VARIABLE FOR LOOPS
	int offset;             // OFFSET USED IN GRAPHICS
	int start=0;
	long int imagesize;		// SIZE OF IMAGE IN PIXELS
	time_t starttime;       // USED TO ESTABLISH SPEED OF
	time_t elapsedtime;	    // PROGRAM EXECUTION
	char outfile[50];       // NAME OF THE OUTPUT FILE
	FILE * fp=NULL;         // FILE POINTER FOR THE OUTPUTFILE
	char *note;             // SINGLE NOTE STRING
	char * retval;          // RESULTS OF ANALYSIS
	retval=new char[400];   // (ALL THE NOTES CONCATENATED)
	strcpy(retval,""); // INITIALISE THE RESULTS VALUE
	section.left=0;
	section.top=0;
	clrscr();
	doswindows();
	setupscreen();
	flag=getdropdown(10,11,1,"\nUse *g*raphics\nUse *t*ext\nUse *n*either\nQuit   -   *ESC*");
	switch(flag)
		{	case 1:{grflag=1;coflag=0;break;}
			case 2:{grflag=0;coflag=1;break;}
			case 3:{grflag=0;coflag=0;break;}
			case 4:
			case ESC:{exit(0);}
		}

	sample.loadpage();  // LOAD THE IMAGE DATA
	if (((grflag==1)&&(sample.findwidth()>630))||
		((grflag==1)&&(sample.findlength()>430)))
			{	writeerror(20,10,"This image is too big to fit on screen. Press ESC");
				grflag=0;
				coflag=1;
			}
	putwindow(10,3,"Please enter the output file name...");
	while (fp==NULL)
		{	gotoxy(1,2);
			readline(outfile,20);
			strcat(outfile,".MUS");
			fp=fopen(outfile,"w");
			if (fp==NULL)
				writeerror(20,10,"You have entered an invalid filename. Press ESC");

		}
	close_window();
	open_window(30,4,75,21,2,2,MESSAGECOLOUR,MESSAGECOLOUR);
	cprintf("Working...\r\n");
	if (grflag)           	 // All BELOW IN GRAPHICS MODE
		{	gettext(1,1,80,25,&screensave);
			graphicsmode();
		}

	starttime=time(NULL);
	while(section=sample.trace(section.left,section.top,grflag,coflag),(section.perimeter!=0))
	 {	start=section.left;
		offset=(320-(sample.findwidth()/2))+section.left;
													   // CALCULATE GRAPHICS
													   // OFFSET
		horz=sample.makehmap(section,grflag,coflag,20);   // MAKE A HORIZONTAL
		spos=horz.extractstpos(coflag); // DENSITY MAP AND EXTRACT THE
										// STAVE POSITIONS
		if (grflag)
			{   setfillstyle(SOLID_FILL,BLACK);
				bar(0,422,640,447);
				setcolor(WHITE);
				outtextxy(320,430,"Perimeter Trace and Horizontal Density Map");
				setcolor(WHITE);
				WAITASEC;
				cleardevice();
				gotoxy(1,1);
			}
		vert=sample.makevmap(section,grflag,coflag); // MAKE A VERTICAL
													 // DENSITY MAP
		if (grflag)
			{	setcolor(WHITE);
				line(30,400-spos.stdensity,640,400-spos.stdensity);
			}
		notes=vert.extractpast(spos.stdensity,coflag);
		if (grflag)
			{	for (j=0;j<=(notes.howmany-1);j++)
					{	line(offset+notes.start[j],150,offset+notes.start[j],420);
						line(offset+notes.end[j],150,offset+notes.end[j],420);
					}   // MARK THE NOTE POSITIONS
			}
		if (coflag)
			{	cprintf("CLASSIFYING AREAS :\r\n");
			}
		for (j=0;j<=(notes.howmany-1);j++)
			{   if (grflag)
				{	setcolor(LIGHTRED);
					line(offset+notes.start[j],150,offset+notes.start[j],420);
					line(offset+notes.end[j],150,offset+notes.end[j],420);
				}
				section.left=start+notes.start[j];
				section.right=start+notes.end[j];
				note=sample.classify(spos,section,j,grflag,coflag);
				strcat(retval,note); // CLASIFY THE NOTE (IE MINUM - HI C)
				if (coflag)
					{	cprintf("%s\r\n",note);
					}
			}
		if (grflag)
			{   setcolor(WHITE);
				outtextxy(320,430,"Vertical Density Map and Area Analysis");
				WAITASEC;
				cleardevice();
			}
		section.left=section.right+5;
		section.top=section.bottom+5;
		strcat(retval," [EOL] ");
	 }
	 elapsedtime=time(NULL)-starttime;
	 imagesize=(sample.findwidth())*(sample.findlength());
	 if (grflag)
		{   settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
			outtextxy(320,430,"Press a key");
			WAITASEC; // EXIT GRAPHICS MODE IF NECESSARY
			closegraph();
			puttext(1,1,80,25,&screensave);
			window(30,4,75,21);
			gotoxy(1,5);
		}
	if (!coflag)
		gotoxy(1,5);
	cprintf("RESULTS OF ANALYSIS:\r\n");
	cprintf("%s\r\n",retval);
	cprintf("A %5ld pixel image took %ld seconds to\r\n",imagesize,elapsedtime);
	printf("analyse including screen I/O operations.\r\n");
	flag=getdropdown(65,5,1,"\n*P*lay\nE*x*it");
	if  (flag==1)
		play(retval);
	fprintf(fp,"%s",retval);
	exit(0);
	return(0);
	}
