/*
                    GRAPHICAL COMMAND INTERPRETOR



                            VERSION 1.0



                                BY


                   BRYAN DUGGAN AND RICHARD BRENNAN


                               WMT3


                   FOR MARK DEEGAN  7th JANUARY 1993        */




#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <dir.h>
#include <graphics.h>
#include <stdlib.h>
#include "windows.h"  /* HEADER FILE CONTAINING SOME HANDY WINDOWS FUNCTIONS */
#include "linklist.h" /* FUNCTIONS TO EDIT LINKED LISTS */
#include "editor.c"   /* SCREEN EDITING MODULE */
#include "drawlist.c" /* DRAWING MODULE */





 /*********************************************************************
 *                                                                    *
 *                 FUNCTION PROTOTYPES                                *
 *                                                                    *
 **********************************************************************/

void writefile(void);
void checksave(void);
void help(void);
void write_list(void);
int is_valid(char string[50]);
int answerwind(int x,int y,int a,int b,char *line);
int initialize(void);
void readfile(void);
void changedirect(void);
void setupscreen(char *options);
void programloop(void);
void initializegdirectory(void);


void checksave(void)
{ char ch=' ';
  ch=answerwind(1,2,'Y','N',"  File in memory may not be saved!! Save (Y/N) ?");
  if (ch=='Y')
     writefile();
}




void help(void)
{ gettextinfo(&screen);
  open_window(10,5,70,20,2,2,MYCOLOUR,MYCOLOUR);
  clrscr();
  gotoxy(25,2);
  cputs("Help");
  gotoxy(11,4);
  cputs("Screen co-ordinates ...   0<x<640  0<y<480.");
  gotoxy(11,5);
  cputs("Colours must be between 0 and 15.");
  gotoxy(11,6);
  cputs("Fills must be between 0 and 7.");
  gotoxy(11,7);
  cputs("If a file is set up in an editor, each command");
  gotoxy(11,8);
  cputs("must be followed by a carriage return.");
  gotoxy(11,9);
  cputs("All files have the postfix .BOB");
  gotoxy(11,10);
  cputs("The turbo C file EGAVGA.BGI must be in the");
  gotoxy(11,11);
  cputs("current directory for pictures to be drawn.");
  gotoxy(11,12);
  cputs("Escape will always solve the problem.");
  getch();
  close_window();
}


/* ACCEPTS X,Y COORDS WHERE AND OPENS A WINDOW TO WRITE A MESSAGE AT
   THAT SCREEN POSITION AND READ IN A CHARACTER AFTER THE STRING COMPARING
   TO THE TWO PASSED CHARACTERS. USED FOR (Y/N) ANSWERS ETC).
   IT RETURNS THE CHARACTER TYPED AFTER VERIFYING IT. */

int answerwind(int x,int y,int a,int b,char *line)
{ int ch=' ';
  open_window(15,12,65,12,2,2,ERRORCOLOUR1,ERRORCOLOUR1);
  gotoxy(x,y);
  strcat(line," ");
  cputs(line);
  while ((ch!=a)&&(ch!=b))
     { ch=getch();
       ch=toupper(ch);
     }
  close_window();
  return(ch);
}










  /**********************************************************************
 *  DEVELOPEMENT TOOL NOT ACTUALLY CALLED ANYWHERE IN THE PROGRAM     *                                                               *
 *                                                                    *
 **********************************************************************/



void write_list(void)
{
 struct listtp *current=head;

 clrscr();
 if (is_empty())
    {
     printf("list is empty");
     pause();
     return;
    }
    else
        {
         printf("\n\n\nThis is the list.\n\n\n\n");
         do
           {
            printf("node %s\n",current->node);
            current=current->next;
           }
         while (current!=head);
         pause();
        }
 }



 /* File functions.*/

/*   CHECKS THE LOADED STRING AND ENSURES THAT IT HAS THE PROPER
     FORMAT.*/






int is_valid(char string[40])
{
 int x1,y1,x2,y2;
 if (sscanf(string,"line (%d %d %d %d )\n",&x1,&y1,&x2,&y2))
    return(0);
 if (sscanf(string,"box (%d %d %d %d )\n",&x1,&y1,&x2,&y2))
    return(0);
 if (sscanf(string,"circle (%d %d %d )\n",&x1,&y1,&x2))
    return(0);
 if (sscanf(string,"pixel (%d %d %d )\n",&x1,&y1,&x2))
    return(0);
 if (sscanf(string,"colour (%d )\n",&x1))
    return(0);
 if (sscanf(string,"fill (%d %d %d %d )\n",&x1,&x2,&y1,&y1))
    return(0);
 if (sscanf(string,"poly (%d %d )\n",&x1,&x2))
    return(0);
 return(1);
}




 /**********************************************************************
 *                                                                    *
 *  READS FILE FROM DISK AND INSERTS INTO THE LINKED LIST
    CALLS THE VALIDATION FUNCTION ABOVE BEFORE INSERTION CAN OCCUR.
 *                                                                    *
 **********************************************************************/






void readfile(void)
{
 char input_string[40],file_name[30],completename[50];

 FILE *in;
 int err_flag=0;
 if (!(is_empty()))
     checksave();

 do
   {
   putwindow(16,2,"          Load what File ?          ");
    gotoxy(1,2);
    if (!(readline(input_string,30)))    /* IF ESC IS PRESSED */
       {
         close_window();
         return;
       }
    else
       {
        strcpy(file_name,input_string);
        strcat(file_name,".BOB");
        strcpy(completename,gdirectory);
        strcat(completename,file_name);       /* APPEND .BOB */
        gotoxy(1,2);                          /* & CURRENT DIRECTORY */
        cputs(completename);
        in=fopen(completename,"r");
        if (in==NULL)                        /* CANNOT OPEN FILE */
        {
           writeerror(30,14,"Invalid Filename. Press ESC.");
        }
        else
        {
         delete_all();
         while (fgets(input_string,30,in))   /* NOT EOF */
              if ((is_valid(input_string))==0)      /* VALIDATE INPUT */
                 append(input_string);              /* INSERT INTO LIST */
                    else
                 err_flag=1;
      }
    }
    if (err_flag==1)
        writeerror(30,13,"Spurious Elements Ignored. Press ESC.");
    close_window();

   }
   while(in==NULL);

     fclose(in);
     strcpy(gfilename,file_name); /* CHANGE THE GLOBAL FILENAME */
}


 /**********************************************************************
 *                                                                    *
 *  WRITES THE CONTENTS OF THE LIST TO A FILE                      *
 *                                                                    *
 **********************************************************************/




void writefile(void)
{
 int flag=FALSE;
 char file_name[40],input_string[40],completename[50];
 struct listtp *current=head;
 FILE *out;
 if (is_empty())
 {
  writeerror(25,13,"No elements to save. Press ESC");
  return;
 }
 else
 do
 {
  if (strcmp("NONAME.BOB",gfilename)==0)    /* IF IT IS A NEW FILE */
     {
      putwindow(16,2,"Please enter file name to write to : ");
       gotoxy(1,2);
       if (!(readline(input_string,38)))      /* ENTER A NEW NAME */
          {
           close_window();
           return;
          }
       else
          {
           strcpy(file_name,input_string);
           strcat(file_name,".BOB");         /* APPEND .BOB */
           strcpy(gfilename,file_name);
          }
       flag=TRUE;
      }
  strcpy(completename,gdirectory);
  strcat(completename,gfilename);           /* APPEND DIRECTORY */
  out=fopen(completename,"w");
  if (out!=NULL)
       do
       {
        fputs(current->node,out);      /* WRITE TO DISK */
        current=current->next;
       }
       while (current!=head);
  else
      {
       writeerror(30,16,"Invalid Filename. Press ESC.");
       strcpy(gfilename,"NONAME.BOB");
       if (flag==TRUE)
   close_window();
   flag=FALSE;
      }

}
while(out==NULL);
if (flag==TRUE)
   close_window();
 fclose(out);
}


 /**********************************************************************
 *                                                                    *
 * CHANGES THE DIRECTORY TO WRITE TO AND READ FROM                     *
 *                                                                    *
 **********************************************************************/

void changedirect(void)
{
 char inputstring[30],currentdirect[30];
 int flag;

 getcurdir(0,currentdirect);      /* ESTABLISH CURRENT DIRECTORY */
 do
   {
    putwindow(16,2,"         Enter new directory :     ");
    gotoxy(1,2);
    if (!(readline(inputstring,30)))       /* NOT ESC */
       {
         close_window();
         return;
       }
    else
       {
        flag=chdir(inputstring);
        if (flag)           /* INVALID DIRECTORY */
           {
            writeerror(39,14,"Invalid Directory");
           }
        else
            {
             strcpy(gdirectory,inputstring);
             if (gdirectory[strlen(gdirectory)-1]!='\\')
                strcat(gdirectory,"\\");         /* SET THE GLOBAL DIRECTORY STRING */
            }
       }
   }
 while(flag);
 chdir(currentdirect);     /* CHANGE BACK TO CURRENT DIRECTORY */
 close_window();
}

/****************************************************************
 *                                                               *
 *                   Menu and Option functions.                  *
 *                                                               *
 *****************************************************************/

/* SETS UP THE SCREEN MENU STRINGS AND DRAWS THE TOP AND BOTTOM BARS */


void setupscreen(char *options)
     {
      char message[80];
      textbackground(LIGHTGRAY);
      clrscr();
      strcpy(options,"\nLoad a picture\nSave current picture\nChange Directory");
      strcat(options,"\nStart new picture\nEdit current picture\nDraw current picture");
      strcat(options,"\nHelp                \nExit             *ESC*");
      strcpy(message,"Current Directory - ");
      strcat(message,gdirectory);
      strcat(message,"   Current File : ");
      strcat(message,gfilename);
      topbar("GCI - Graphics Command Interpretor (c) 1993 Bryan Duggan & Richard Brennan");
      bottombar(message);
}


 /**********************************************************************
 *  CASES THE GET_OPTION FUNCTION AND CALLS THE APPROPRIATE
    FUNCTION                                                               *
 *                                                                    *
 *                                                                    *
 **********************************************************************/


void programloop(void)
{
 int choice=1;

 while (choice!=ESC)        /* UNTIL ESC IS RETURNED */
       {
       char options[200];
       setupscreen(options);
       choice=getdropdown(10,9,choice,options);   /* DRAW THE DROP MENU */
       switch(choice)
               {
                case 1:{readfile();break;}
                case 2:{writefile();break;}
                case 3:{changedirect();break;}
                case 4:{
                        if (!(is_empty()))
                           checksave();
                        delete_all();
                        strcpy(gfilename,"NONAME.BOB");
                        break;}
                case 5:{editor();break;}
                case 6:{drawpicture();break;}
                case 7:{help();break;}
                case 8:{choice=ESC;break;}
               }
       close_window();
       close_window();
       close_window();
       close_window();
       }

}

void initialisegdirectory(void)       /* INITIALISES THE DIRECTORY AT */
     {                                /* PROGRAM INVOCATION */
      char drive[4],directory[12];
      drive[0]=(getdisk()+65);
      drive[1]=':';
      drive[2]='\\';
      drive[3]='\0';
      getcurdir(0,directory);
      strcpy(gdirectory,drive);
      strcat(gdirectory,directory);
      if (gdirectory[strlen(gdirectory)-1]!='\\')
         strcat(gdirectory,"\\");
}


main()

{
 doswindows();  /* INITIALISES VARIABLES USED IN THE DRAWING OF THE WINDOWS */
 intro();   /* DRAWS AN INTERESTING INTRO SCREEN AND WAITS FOR USER RESPONSE */
 head=NULL;
 clrscr();
 strcpy(gfilename,"NONAME.BOB");
 initialisegdirectory();
 programloop();
 if (!is_empty())
    checksave();
 close_window();
}

