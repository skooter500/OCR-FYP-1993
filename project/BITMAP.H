// INTERFACE FILE



#define FAIL 0
#define OK 1
#define TRUE 1
#define FALSE 0
#define ON 0
#define OFF 1
#define DEFAULT 1
#define BARWIDTH 7
#define NOISE 20
#define STAVELINE 5
#define CLEFWIDTH 30
#define CROCHETWIDTH 30
#define PIXELSINMINUM 210
#define LOWC 330
#define      MIDCSH 330
#define LOWD 370
#define      MIDDSH 330
#define LOWE 416
#define LOWF 441
#define      MIDFSH 330
#define LOWG 494
#define      MIDGSH 330
#define LOWA 555
#define      MIDASH 330
#define LOWB 623
#define HIC  660
#define HID  700
#define HIE  746
#define HIF  771
#define HIG  824
#define MINUM 500
#define CROCHET 250
#define QUAVER 125
#define WAITASEC getch()
#define waitESC while(getch()!=27)
#define GRAPH_DIR ""

typedef unsigned char byte;


struct stavepositions
	{	int start[5];
		int end[5];
		int stdensity;
	};

struct segment
	{  long top,bottom,left,right;
	   int perimeter;
	};

class bitmap // base class
	  {    protected :
			long int width,length; //in bits
			long int bytewidth; // width in bytes
			byte far *bm[400];
			public :
			bitmap(long xsize,long ysize);  // constructer
			int resize(long xsize,long ysize);
			void setbit(long xval,long yval);
			long findwidth();
			long findlength();
			void clearbit(long xval,long yval);
			int getbit(long xval,long yval);
			void setbyte(long xbyte,long ybyte,byte value);
			void showbitmap(void);
			void showbytes(void);
			~bitmap();    // destructer
	  };

struct possibles
	{	long start[30];
		long end[30];
		int howmany;
	};

class page:public bitmap     // derived from bitmap
	{	private :
			char filename[50];
		public :
			page(long xsize,long ysize, char name[50]):bitmap(xsize,ysize)
			{strcpy(filename,name);}
			void loadpage(void);
			int ison(long xpos,long ypos,int dir);
			segment trace(long xstart,long ystart,int grflag,int coflag);
			char * classify(stavepositions spos,segment area,int which,int grflag,int coflag);
			class hmap makehmap(segment seg,int grflag,int coflag,int gystart);
			class vmap makevmap(segment seg,int grflag,int coflag);
		   void touchup(void);
		   void savepage(void);
		   void displaypage(void);
		   int  pixelcount(segment seg,int coflag);       
		   char * recognise(int grflag,int coflag);

	};
class mask:public bitmap // derived from bitmap
	{	private :
			char *filename;
		public :
			void loadmask(char *newfile);
			int compare(segment seg,int flag);
	};

class hmap
	{   friend page;
		private :
			int *density,length;
		public :
			stavepositions extractstpos(int coflag);
			possibles extractpast(int threshold,int coflag);
			void drawmap(int gxstart,int gystart);


	};

class vmap
	{  friend page;
		private :
		int *density,width;
		public :
			possibles extractpast(int threshold,int coflag);
			void drawmap(int gxstart,int gystart);
	};
void graphicsmode(void);

