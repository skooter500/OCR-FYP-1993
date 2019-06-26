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
