// IMPLIMENTATION FILE

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




inline int mod(long num,long denom)  // RETURNS REMAINDER (MOD)
	{	return (ldiv(num,denom).rem);
	}



// CONSTRUCTER FUNCTION FOR BITMAP CLASS
bitmap::bitmap(long xsize,long ysize)
	   {    long x,y,i;

			length=ysize-1;
			width=xsize-1;
			bytewidth=((width+1)/8)+((ldiv((width+1),8).rem==0) ? 0:1)-1;
			for (i=0;i<=bytewidth;i++)
				{ 	if ((bm[i] = (byte far *) farmalloc(length + 1)) == NULL)
					{
					writeerror(20,10,"Error : INSUFFICIENT MEMORY FOR GRAPHICS DATA \a");
					exit(1);
					}

				}
	   }


long bitmap::findwidth() // RETURNS THE WIDTH OF A BITMAP IN PIXELS
	{	return(width);
	}

long bitmap::findlength() // RETURNS THE LENGTH OF A BITMAP IN PIXELS
	{	return(length);
	}


// THIS FUNCTION REALLOCATES THE SPACE THAT WAS ALLOCATED IN THE CONSTRUCTER
// AND ADJUSTS THE "WIDTH" AND "LENGTH" VARIABLES APPROPRIATLY.
int bitmap::resize(long xsize,long ysize)
	   {    long x,y,i;

			for (i=0;i<=bytewidth;i++)
				farfree(bm[i]);
			length=ysize-1;
			width=xsize-1;
			bytewidth=((width+1)/8)+((ldiv((width+1),8).rem==0) ? 0:1)-1;
			for (i=0;i<=bytewidth;i++)
				{   if ((bm[i] = (byte far *) farmalloc(length + 1)) == NULL)
					{	return (FAIL);
					}
				}
			return (OK);
		}




bitmap::~bitmap() // DESTRUCTOR FUNCTION. FREES ALL ALLOCATED MEMORY
	   {	int i;
			for (i=0;i<=bytewidth;i++)
				{   farfree(bm[i]);
				}

	   }



//  FUNCTION TO SET A GIVEN BIT IN THE BITMAP (TO THE VALUE 1)
void bitmap::setbit(long xval,long yval)
  {   int x,y,value;
	  x=xval/8;    // CALCULATE X POSITION IN ARRAY
	  y=yval;      // Y POSITION IN ARRAY
	  switch ( (int) xval-(x*8))
		{	case 7:{value=1;break;}   /* BINARY 00000001 IE.BIT 0 */
			case 6:{value=2;break;}   /* BINARY 00000010 IE.BIT 1 */
			case 5:{value=4;break;}   /* BINARY 00000100 IE.BIT 2 */
			case 4:{value=8;break;}   /* BINARY 00001000 IE.BIT 3 */
			case 3:{value=16;break;}  /* BINARY 00010000 IE.BIT 4 */
			case 2:{value=32;break;}  /* BINARY 00100000 IE.BIT 5 */
			case 1:{value=64;break;}  /* BINARY 01000000 IE.BIT 6 */
			case 0:{value=128;break;} /* BINARY 10000000 IE.BIT 7 */
		}
	  bm[x][y]|=value; // BINARY "OR" THE ARRAY VALUE WITH THE NEW VALUE
					   // WHICH WILL SET THE APPROPRIATE BIT TO 1
  }


//  FUNCTION TO CLEAR A GIVEN BIT IN THE
//	BITMAP IE. (MAKE IT'S VALUE THE VALUE 0)
void bitmap::clearbit(long xval,long yval)
  {	 long x,y;
	 int value;
	 x=xval/8;    /* CALCULATE X POSITION IN ARRAY */
	 y=yval;      /* CALCULATE Y POSITION IN ARRAY */
	 switch (xval-(x*8))
	  {		case 0:{value=127;break;}   /* BINARY 11111110 IE.BIT 0 */
			case 1:{value=191;break;}   /* BINARY 11111101 IE.BIT 1 */
			case 2:{value=223;break;}   /* BINARY 11111011 IE.BIT 2 */
			case 3:{value=239;break;}   /* BINARY 11110111 IE.BIT 3 */
			case 4:{value=247;break;}   /* BINARY 11101111 IE.BIT 4 */
			case 5:{value=251;break;}   /* BINARY 11011111 IE.BIT 5 */
			case 6:{value=253;break;}   /* BINARY 10111111 IE.BIT 6 */
			case 7:{value=254;break;}   /* BINARY 01111111 IE.BIT 7 */
	  }
   bm[x][y]&=value;  /* BINARY "AND" THE ARRAY VALUE WITH THE NEW VALUE
				WHICH WILL SET THE APPROPRIATE BIT TO 0 */
  }


/* RETURNS VALUE OF A REQUIRED BIT IN A BITMAP (EITHER 1 OR 0) */
int bitmap::getbit(long xval,long yval)
  {		int x,y,value;
		x=(int) (xval/8);  /* CALCULATE X POSITION IN ARRAY */
		y=(int) (yval);    /* CALCULATE Y POSITION IN ARRAY */
		switch ((int) xval-(x*8))      /* WHICH BIT IS REQUIRED ? */
			{	case 7:{value=1;break;}   /* BINARY 00000001 IE.BIT 0 */
				case 6:{value=2;break;}   /* BINARY 00000010 IE.BIT 1 */
				case 5:{value=4;break;}   /* BINARY 00000100 IE.BIT 2 */
				case 4:{value=8;break;}   /* BINARY 00001000 IE.BIT 3 */
				case 3:{value=16;break;}  /* BINARY 00010000 IE.BIT 4 */
				case 2:{value=32;break;}  /* BINARY 00100000 IE.BIT 5 */
				case 1:{value=64;break;}  /* BINARY 01000000 IE.BIT 6 */
				case 0:{value=128;break;} /* BINARY 10000000 IE.BIT 7 */
			}
	if ((bm[x][y]&value)==value)  /* BINARY "AND" TO CHECK IS THE */
	   return(1);                  /* BIT SET  */
	else
		return(0);

  }

// SETS AN ENTIRE  BYTE IN A BITMAP. THIS IS MORE EFFICIENT THAN CALLING
// SETBIT 8 TIMES
inline void bitmap::setbyte(long xbyte,long ybyte,byte value)
	{	bm[xbyte][ybyte]=value;
	}

// SHOWS THE BITMAP AS 1'S AND 0'S IN TEXT MODE
void bitmap::showbitmap(void)
   {	int xs,ys;

		for (ys=0;ys<=length;ys++)
			{	for (xs=0;xs<=width;xs++)
				if (this->getbit(xs,ys)==1)
					printf("1");
				else
					printf("0");
				printf("   %d,%d\n",xs,ys);
			}
  }


// SHOWS THE ACTUAL BYTES IN A BITMAP IN TEXT MODE. THIS AND THE PREVIOUS
// FUNCTION WERE USEFUL IN THE DEBUGGING OF THE PROGRAM
void bitmap::showbytes()
	 {	int x,y;

		clrscr();
		printf("width=%d...Length=%d\n",bytewidth,length);
		for(y=0;y<=length;y++)
			   for (x=0;x<=bytewidth;x++)
				{printf("x=%d...y=%d...VALUE    %d\n",x,y,bm[x][y]);
				 WAITASEC;}
		WAITASEC;
	}


// LOADS A PCX FILE INTO MEMORY, RESIZING THE ALLOCATED MEMORY
// AS NECESSARY.
void page::loadpage(void)
	{	FILE *fp;
		char file[50],completefile[50],temp[2];
		int buffer;
		int xpos=0,ypos=0;
		int x1,y1,x2,y2,bpl,numreps,bytes=0;

		putwindow(8,4,"Please enter the input file name...");
		do
			{	gotoxy(1,2);
				readline(file,25);
				strcpy(completefile,"");
				strcat(completefile,file);
				strcat(completefile,".PCX");
				gotoxy(10-(strlen(completefile)/2),3);
				cprintf("           %s           ",completefile);
				fp=fopen(completefile,"r");
				if (fp==NULL)
					writeerror(20,10,"Invalid filename. Press ESC");
			}
		while  (fp==NULL);
		strcpy(filename,completefile);
		// THE FOLLOWING LINES READ IMPORTANT INFO FROM THE HEADER
		fseek(fp,4,SEEK_SET); // MOVE FILE POINTER TO BYTE 4
		x1=(getc(fp)+(getc(fp)*256));    // READ X1
		y1=(getc(fp)+(getc(fp)*256));    // READ Y1
		x2=(getc(fp)+(getc(fp)*256));    // READ X2
		y2=(getc(fp)+(getc(fp)*256));    // READ Y2
		fseek(fp,66,SEEK_SET); // MOVE FILE POINTER TO BYTE 66
		bpl=(getc(fp)+(getc(fp)*256));   // READ BYTES PER LINE
		if (resize( (x2-x1)+1 ,(y2-y1)+1)==FAIL) // RESIZE MEMORY SPACE
			{	writeerror(20,12,"INSUFFICIENT MEMORY FOR GRAPHICS DATA. Press ESC\a");
				exit(1);
			}
		// THE REST OF THIS SECTION DECODES THE COMPRESSED IMAGE INFO
		// AND STORES IT IN MEMORY
		fseek(fp,128,SEEK_SET); // MOVE TO START OF DATA
		while( (buffer=getc(fp)) !=EOF)
			{   if (buffer==193)     // 11000001 NEXT BYTE IS >63
					{  buffer=getc(fp);
					   bm[xpos][ypos]=buffer;
					   xpos++;
					   bytes++;
					   if (xpos==(bpl))   {xpos=0;ypos++;}
					}
				else
				if (buffer>193)// 11XXXXXX NEXT BYTE IS REPEATED
						{	numreps=(buffer&63);	   // XXXXXX TIMES
							buffer=getc(fp);   // <- READ REPEATED
							while (numreps)            //    BYTE
							{   bm[xpos][ypos]=buffer;
								xpos++;
								bytes++;
								if (xpos==(bpl))   {xpos=0;ypos++;}
								numreps--;
							}
						}
				else
				if (buffer<193)
						{	bm[xpos][ypos]=buffer; // DEFAULT CASE
							xpos++;
							bytes++;
							if (xpos==(bpl))   {xpos=0;ypos++;}
						}

			}  // END WHILE
			putwindow(18,2,"   Image sucessfully loaded and decompressed  ");
			gotoxy(1,2);
			cprintf("    Size after decompression : %5d bytes",bytes);
			WAITASEC;
			close_window();
			close_window();

	} // END FUNCTION



// THIS FUNCTION CHECKS THE STATUS OF A BIT IN THE BITMAP,
// SPECIFIED BY THE DIRECTION PARAMETER. SEE THE TRACE FUNCTION BELOW
int page::ison(long xpos,long ypos,int dir)
	{   switch (dir)
			{	case(0):return(getbit(xpos,ypos+1)==ON);
				case(1):return(getbit(xpos+1,ypos)==ON);
				case(2):return(getbit(xpos,ypos-1)==ON);
				case(3):return(getbit(xpos-1,ypos)==ON);
			}
		return(DEFAULT);
	}


// THIS FUNCTION USES A PERIMETER TRACING ALGORITHM TO TRACE THE OUTLINE
// OF A BLOB OF "ON" BITS IN THE BITMAP. IT RETURNS THE X AND Y
// BOUNDS OF THE BLOB.
segment page::trace(long xstart,long ystart,int grflag,int coflag)
	{	segment seg;
		seg.perimeter=0;
		long xpos=xstart,ypos=ystart;
		int notfound=TRUE;
		int colour=RED;
		int gx=320-(width/2),gy=300-(length/2);

		int dir=0; // START TRACING DOWNWARDS
				   // DOWN=0,RIGHT=1,UP=2,LEFT=3
		while (notfound)  // ESTABLISH START OF TRACE
			{	if (getbit(xpos,ypos)==ON)
					notfound=FALSE;
				else
					{ 	xpos++;
						if (xpos>(width))
							{xpos=0;ypos++;}
						if (ypos>(length-20))
							{
							  if (grflag)
								{ settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
								  outtextxy(320,170,"END OF IMAGE");
								}
								else
								  {	cprintf("END OF IMAGE\r\n");
								  }
							  return(seg);

							}
					}
			}
		if (grflag)
			displaypage();
		if (coflag)
			{	cprintf("PERIMETER TRACING\r\n");
			}
	do
	  {
		while (notfound)  // ESTABLISH START OF TRACE
			{	if (getbit(xpos,ypos)==ON)
					notfound=FALSE;
				else
					{ 	xpos++;
						if (xpos>(width))
							{xpos=0;ypos++;}
						if (ypos>(length-10))
							{  if (grflag)
								{ settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
								  outtextxy(320,170,"END OF IMAGE");
								}
								else
								  {	cprintf("END OF IMAGE\r\n");
								  }
							  seg.perimeter=0;
							  return(seg);
							}
					}
			}
		if (coflag)
			{	cprintf("Found Start\r\n");
			}
		xpos--;  // MOVE BACK TO A BLANK PIXEL
		xstart=xpos;
		ystart=ypos;
		seg.left=xpos;
		seg.top=ypos;
		seg.right=xpos;
		seg.bottom=ypos;
		seg.perimeter=0;

		do
			{       if (ison(xpos,ypos,mod(dir+1,4)))
					{	if (ison(xpos,ypos,dir))
							if (ison(xpos,ypos,mod(dir+3,4)))
								{	dir=mod(dir+2,4); // CUL DE SAC
								}
							else
								dir=mod(dir+3,4); // THERE IS AN OBSTRUCTION
										  // SO TRAVEL AROUND IT
					}
				else
				dir=mod(dir+1,4);  // INCRIMENT THE DIRECTION
				switch(dir)
					{   case 0:{    if (ypos<length)
											ypos++;
									if (ypos>(length-1))
										{ seg.perimeter=0;
										  return(seg);
										}
									if (ypos>seg.bottom)
												seg.bottom=ypos;
									break;
								}
						case 1:{    if (xpos<width)
										xpos++;
									if (xpos>width)
										{ seg.perimeter=0;
										  return(seg);
										}
									if (xpos>seg.right)
										seg.right=xpos;// ALL THIS, MEANS
									break;             // GO FORWARD ONE STEP
								} 					   // IN THE DIRECTION
						case 2:{	if (ypos>0)        // INDICATED BY "DIR".
									ypos--;
									if (ypos<0)
										{ seg.perimeter=0;
										  return(seg);
										}
									if (ypos<seg.top)
										seg.top=ypos;
									break;

								}
						case 3:{	if (xpos>0)
									xpos--;
									if (xpos<0)
										{ seg.perimeter=0;
										  return(seg);
										}
									if (xpos<seg.left)
										seg.left=xpos;
									break;
								}
					}   // END SWITCH/CASE
				seg.perimeter++;
				if (grflag)
					putpixel(gx+xpos,gy+ypos,RED);


			}   // END DO/WHILE
		while ((xpos!=xstart)||(ypos!=ystart));
		xpos=seg.right+2;  	// RESET THESE VARIABLES
		ypos=ystart;  	// IN CASE WE HAVE JUST SEGMENTED NOISE
		notfound=TRUE;  // AND THEREFORE WILL HAVE TO START THE
						// PROCESS AGAIN.
		if (coflag)
			{	cprintf("Found End\r\n");
				cprintf("Perimeter=%d pixels\r\n",seg.perimeter-4);
			}
		if (grflag)
			{   colour=getcolor();
				setcolor(LIGHTRED);
				rectangle(gx+seg.left,gy+seg.top,gx+seg.right,gy+seg.bottom);
				setcolor(colour);
			}
			seg.perimeter-=4;

	  }
	  while(seg.perimeter<NOISE); // REPEAT UNTIL A SEMENT IS FOUND THAT HAS
						   // A PERIMETER OF MORE THAN THE NOISE LEVEL.
						   // THIS SHOULD STOP THE FUNCTION FROM
						   // RETURNING A NOISE SEGMENT.
		return(seg);
	}


// THIS FUNCTION DISPLAYS A GRAPHICAL REPRESENTATION OF THE
// BITMAP
void page::displaypage(void)
	{	long x,y;
		int gx=320-(width/2),gy=300-(length/2);
		for(y=0;y<=length;y++)
			{
			for(x=0;x<=width;x++)
				{
				if (getbit(x,y)==ON)
					{
					putpixel(gx+x,gy+y,DARKGRAY);
					}
				else
					{
					putpixel(gx+x,gy+y,WHITE);
					}
				}
			}
	}

// THIS FUNCTION CONSTRUCTS A HORIZONTAL DENSITY MAP OF A SECTION
// FROM A BITMAP
hmap page::makehmap(segment seg,int grflag,int coflag,int gystart)
	{  int count=0;
	   int colour=LIGHTRED;
	   long x,y;
	   hmap retvalue;
	   int gy=gystart,gx=320-(width/2);

	   if (coflag)
			{	cprintf("MAPPING HORIZONTAL DENSITY...\r\n");
			}
	   retvalue.length=(seg.bottom-seg.top);
	   retvalue.density=new int[(retvalue.length)+2];
	   for (y=seg.top;y<=seg.bottom;y++)
		{	for (x=seg.left;x<=seg.right;x++)
				if (getbit(x,y)==ON)
					count++;
			retvalue.density[y-(seg.top)]=count;
			count=0;
		}
		if (grflag)
			{   colour=getcolor();
				setcolor(CYAN);
				gx+=seg.left;
				for (y=0;y<=retvalue.length;y++)
					line(gx,gy+y,gx+retvalue.density[y],gy+y);
				setcolor(colour);
			}
		return(retvalue);
	}


// THIS FUNCTION MAKES A VERTICAL DENSITY MAP OF A SECTION
// IN THE DENSITY MAP
vmap page::makevmap(segment seg,int grflag,int coflag)
	{  int count=0;
	   int colour=LIGHTRED;
	   long x,y;
	   vmap retvalue;
	   int gx=320-(width/2);

	   if (coflag)
			{	cprintf("MAPPING VERTICAL DENSITY...\r\n");
			}
	   retvalue.width=(seg.right-seg.left);
	   retvalue.density=new int[(retvalue.width)+1];
	   for (x=seg.left;x<=seg.right;x++)
		{	for (y=seg.top;y<=seg.bottom;y++)
				if (getbit(x,y)==ON)
					count++;
			retvalue.density[x-(seg.left)]=count;
			count=0;
		}

		if (grflag)
			{   colour=getcolor();
				setcolor(CYAN);
				for (x=seg.left;x<=seg.right;x++)
					line(gx+x,400,gx+x,400-retvalue.density[x-seg.left]);
				setcolor(colour);
			}
		return(retvalue);
	}

// THIS FUNCTION SEARCHES A HORIZONTAL DENSITY MAP FOR PEEKS THAT
// MATCH THE CHARACHTERISTICS OF STAVE LINES
stavepositions hmap::extractstpos(int coflag)
	{	int threshold=0,lineno=0,max=0,i=0;
		stavepositions retvals;
		retvals.stdensity=0;
		if (coflag)
			{	cprintf("ANALYSING HORIZONTAL DENSITY MAP\r\n");
			}
		for (i=0;i<=length;i++)
			if (density[i]>max)
				max=density[i];     // ESTABLISH THE MAXIMUM DENSITY
		threshold=(int)(max*.6);    // CALCULATE THE THRESHOLD
		i=0; // RESET i
		while (i<length)
			{	if (density[i]>=threshold)       // WE HAVE FOUND A STAVE LINE
					{	retvals.start [lineno]=i;
						while (density[i++]>=threshold); // TRACE DOWNWARDS
						retvals.end [lineno]=i;		   	// TO THE NEXT BLANK
						lineno++;                       // LINE
					}
				i++;
			}
		if (lineno!=5)
			{	closegraph();
				writeerror(12,12,"ERROR : Invalid number of stavelines found.  Press ESC");
				exit(1);
			}

		if (coflag)
			{	cprintf("Staves located at :\r\n");
				for (i=0;i<=4;i++)
					{	cprintf("%4d %4d\r\n",retvals.start[i],retvals.end[i]);
					}
			}
			retvals.stdensity=14;  // THIS VALUE IS A CONSTANT BUT IS
								   // STOED AS A VARIABLE IN ORDER TO
								   // FACILITATE FUTURE MODIFICATIONS TO
								   // THE PROGRAM CODE, WHERE THIS VALUE
								   // MAY HAVE TO CHANGE.
		return(retvals);
	}


// COUNTS THE PIXELS IN A GIVEN REGION
int  page::pixelcount(segment seg,int coflag)
	{	int x,y,count=0;
		for (x=seg.left;x<=seg.right;x++)
			for (y=seg.top;y<=seg.bottom;y++)
				if (getbit(x,y)==ON)
					count++;
		if (coflag)
			{	cprintf("PIXEL COUNT RETURNS - %d\r\n",count);
			}
		return(count);
}


// THE FOLLOWING TWO FUNCTIONS ARE USED TO SEARCH
// THE DENSITY MAPS FOR OCCURANCES OF PEEKS PAST A CERTAIN
// THRESHOLD.
possibles vmap::extractpast(int threshold,int coflag)
	{	possibles retval;
		long i=0,j=0;
		int lineno=0;
		int max=0;

		while (i<width)
			{
				if (density[i]>threshold)  		// WE HAVE FOUND THE START
					{	retval.start[lineno]=i; // OF A PEEK
						while (density[i++]>threshold); // FIND THE END OF THE PEEK
						retval.end[lineno]=i;
						i++;
						for (j=retval.start[lineno];j<=retval.end[lineno];j++)
							if (density[j]>max)    // IF THE SECTION
								max=density[j];    // IS TOO SMALL
						if (max>(threshold+NOISE)) // THEN REJECT IT
							lineno++;
						max=0;
					}
				i++;
			}
		retval.howmany=(lineno);
		if (coflag)
			{	cprintf("ANALYSING MAP\r\n");
				cprintf("%d area(s) were found at :\r\n",retval.howmany);
				for (i=0;i<=(retval.howmany-1);i++)
					{	cprintf("%4ld %4ld\r\n",retval.start[i],retval.end[i]);
					}
			}
		return (retval);
	}



possibles hmap::extractpast(int threshold,int coflag)
	{	possibles retval;
		long i=0;
		int lineno=0;

		while (i<length)
			{
				if (density[i]>threshold)
					{	retval.start[lineno]=i;
						while (density[i++]>threshold);
						retval.end[lineno]=i;
						i++;
					  if((retval.end[lineno]-retval.start[lineno])>STAVELINE)
						lineno++;
					}
				i++;
			}
		retval.howmany=(lineno);
		if (coflag)
			{	cprintf("ANALYSING HORIZONTAL DENSITY MAP\r\n");
				cprintf("%d area(s), found at :\r\n",retval.howmany);
				for (i=0;i<=(retval.howmany-1);i++)
					{	cprintf("%4ld %4ld\r\n",retval.start[i],retval.end[i]);
					}
			}
		return (retval);
	}

// THIS FUNCTION COMPARES THE POSITION OF A BLOB WITH THE STAVE LINES
// AND RETURNS A NOTE VALUE
char *whereis(stavepositions spos,long middle,long top,long bott)
	{
	   // CHECK TO SEE IF THE BLOB LIES ON THE STAVE LINES
	   // IE IF IT IS A E, G, B, D OR F

	   if ((top<(spos.start[4]))&&(top>(spos.end[3]))&&(bott>(spos.end[4])))
				return (" [LOW E] ");
	   if ((top<(spos.start[3]))&&(top>(spos.end[2]))&&(bott>(spos.end[3]))&&
		  (bott<(spos.start[4])))
				return (" [LOW G] ");
	   if ((top<(spos.start[2]))&&(top>(spos.end[1]))&&(bott>(spos.end[2]))&&
		  (bott<(spos.start[3])))
				return (" [LOW B] ");
	   if ((top<(spos.start[1]))&&(top>(spos.end[0]))&&(bott>(spos.end[1]))&&
		  (bott<(spos.start[2])))
				return (" [HI D] ");
	   if ((top<(spos.start[0]))&&(bott>(spos.end[0]))&&(bott<(spos.start[1])))
				return (" [HI F] ");

	   // CHECK TO SEE IF THE BLOB LIES BETWEEN ANY OF THE STAVE LINES
	   // IE IF IT IS THE NOTE D, F, A, C OR E

	   if (middle>(spos.end[4]))
			return(" [LOW D] ");
	   if ((middle<(spos.start[4]))&&(middle>(spos.end[3])))
			return(" [LOW F] ");
	   if ((middle<(spos.start[3]))&&((middle>spos.end[2])))
			return(" [LOW A] ");
	   if ((middle<(spos.start[2]))&&(middle>(spos.end[1])))
			return( " [HI C] ");
	   if ((middle<(spos.start[1]))&&((middle>spos.end[0])))
			return(" [HI E] ");
	   if (middle<(spos.start[0]))
			return(" [HI G] ");


	   return (" [UNKNOWN] "); // IF IT IS NONE OF THE ABOVE
	}

// THIS FUNCTION USES DENSITY MAPPING TO DETERMINE IF A NOTE IS A BAR LINE,
// A CLEF, A MINUM,A CROCHET OR A QUAVER.
char * page::classify(stavepositions spos,segment section,int which,int grflag,int coflag)
	{  hmap	map1;
	   vmap map2;
	   possibles blob,bar;
	   long threshold=0,max=0,maxpos=0;
	   char retvalue[20];
	   int i=0,j=0;
	   int middle=0,top=0,bott=0,middlemap, diff=0;
	   segment quaverblobs[2],otherblob;
	   int gy=1,gx=320-(width/2);
	   int numpixels=0;

	   if ( (which==0) && ((section.right-section.left)>=CLEFWIDTH) )
		return(" [CLEF] ");
	   if ((section.right-section.left)<=BARWIDTH)
			{	return(" [BAR]\r\n");    // THE EASY BITS
			}

	   map1=makehmap(section,grflag,coflag,5);     // H-DENSITY MAP THE REGION
	   if ((section.right-section.left)>CROCHETWIDTH) // PAIR OF QUAVERS
			{   strcpy(retvalue," DOUBLE QUAVER-");
				middlemap=(map1.length/2);
				for (i=0;i<=map1.length;i++)  // CALCULATE THE MAXIMUM DENSITY
				if (map1.density[i]>max)
					{	max=map1.density[i];
					}
				threshold=max*.3;    // CALCULATE THE THRESHOLD
				blob=map1.extractpast(threshold,0); // SEARCH FOR BLOBS
				threshold=max*.5;  // INCREACE THE THRESHOLD
				bar=map1.extractpast(threshold,0); // SEARCH USING  THE
				// INCREACED THRESHOLD.THIS SHOULD RETURN THE POSITION
				// OF THE BAR
				maxpos=bar.end[0];


				if (maxpos>=middlemap)  // QUAVERBAR IS UNDER THE NOTE BLOBS
					{	quaverblobs[0].top=(section.top);
						quaverblobs[0].bottom=(section.top+blob.start[blob.howmany-1]-3);
						quaverblobs[1].top=(section.top);
						quaverblobs[1].bottom=(section.top+blob.start[blob.howmany-1]-3);
/*ELIMINATE THE BAR*/   section.bottom=(section.top+bar.start[0]-3);
						diff=0;
					}
				else    //QUAVERBAR IS ABOVE THE NOTE BLOBS
					{   quaverblobs[0].top=(section.top+bar.end[0]+3);
						quaverblobs[0].bottom=(section.bottom);
						quaverblobs[1].top=(section.top+bar.end[0]+3);
						quaverblobs[1].bottom=(section.bottom);
						diff=bar.end[0]+3;
/*ELIMINATE THE BAR*/	section.top=section.top+(bar.end[0]+3);

					}
				max=0;
				map2=makevmap(section,0,0);
				for (i=0;i<=map2.width;i++)  // CALCULATE THE MAXIMUM DENSITY
				if (map2.density[i]>max)
					{	max=map1.density[i];
						maxpos=i;
					}
				threshold=max*.25;    // CALCULATE THE THRESHOLD
				blob=map2.extractpast(threshold,0); // WE HAVE NOW HOPEFULLY
				quaverblobs[0].left=section.left+blob.start[0];  // EXTRACTED LOCATION
				quaverblobs[0].right=section.left+blob.end[0];   // OF THE 2 BLOBS
				quaverblobs[1].left=section.left+blob.end[0]+3;  // IN THE QUAVER.
				quaverblobs[1].right=section.right;
				for (j=0;j<=1;j++)    // REPEAT FOR BOTH BLOBS
					{  	max=0;
						map1=makehmap(quaverblobs[j],grflag,0,100); // H-DENSITY MAP
						for (i=0;i<=map1.length;i++)  // CALCULATE THE MAXIMUM DENSITY
						if (map1.density[i]>max)
							max=map1.density[i];
						threshold=max*.32;    // CALCULATE THE THRESHOLD
						blob=map1.extractpast(threshold,coflag);
						if  (blob.howmany<1)// IF ZERO BLOBS
							{	strcat(retvalue," [UNKNOWN] ");
								return(retvalue);
							}
						middle=(((blob.start[0]+((blob.end[0]-blob.start[0])/2)))+diff);
						top=(blob.start[0])+diff;
						bott=(blob.end[0])+diff;
						if (grflag)
							{	line(gx+quaverblobs[j].left,gy+top-diff+97,gx+quaverblobs[j].right,gy+top-diff+97);
								line(gx+quaverblobs[j].left,gy+middle-diff+97,gx+quaverblobs[j].right,gy+middle-diff+97);
								line(gx+quaverblobs[j].left,gy+bott-diff+97,gx+quaverblobs[j].right,gy+bott-diff+97);
							}
						strcat(retvalue,whereis(spos,middle,top,bott));
					}
				return(retvalue);
			}
	   else // THIS ELSE REFERS BACK TO " IF IT IS A DOUBLE QUAVER"
			// MEANING IF IT IS SOMETHING OTHER THAN A DOUBLE QUAVER
		 {	for (i=0;i<=map1.length;i++)  // CALCULATE THE MAXIMUM DENSITY
				if (map1.density[i]>max)
					max=map1.density[i];
		   threshold=max*.4;    // CALCULATE THE THRESHOLD
		   blob=map1.extractpast(threshold,coflag);
		   if  (blob.howmany!=1) // IF MORE THAN ONE BLOB IN THE SECTION
				{	return(" [UNKNOWN] ");
				}
		   middle=(blob.start[0]+((blob.end[0]-blob.start[0])/2));
		   top=blob.start[0];
		   bott=blob.end[0];
		   if (grflag)
				{	line(gx+section.left,gy+top+3,gx+section.right,gy+top+3);
					line(gx+section.left,gy+middle+3,gx+section.right,gy+middle+3);
					line(gx+section.left,gy+bott+3,gx+section.right,gy+bott+3);
				}
		   otherblob.left=section.left;
		   otherblob.right=section.right;
		   otherblob.top=section.top+top;
		   otherblob.bottom=section.top+bott;
		   if (grflag)
				numpixels=pixelcount(otherblob,0);
		   else
				numpixels=pixelcount(otherblob,coflag);
		   if (numpixels>PIXELSINMINUM)
				strcpy(retvalue," CROCHET -");
		   else
				strcpy(retvalue," MINUM -");
		   strcat(retvalue,whereis(spos,middle,top,bott));
		   return(retvalue);
		 }  // END ELSE
	}