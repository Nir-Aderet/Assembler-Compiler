#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include "firstwalk.h"
#include "structs.h"
#include "sidefunctions.h"
#include "secondwalk.h"




/* ?????? maybe send aFILE *fp instead of char *path ?????? */

int secwalk(FILE *infp, struct global *g, struct LTNode *head, char *path)
{
	/* ***************************************************************************************************************************** */
	
	FILE *fp;
	
	char *newname = (char *) malloc (strlen(path) +4 );
	
	strcpy(newname,path);
	strcat(newname,".ob");
	fp = fopen(newname,"w");	/* Try to open file */

	/* ***************************************************************************************************************************** */

	int dtimlen = (((g->dc)*21) + ((g->dc)*MAX_LINE_LENGTH) + ((g->dc)*12));

	char *token;				/* pointer for representing each word in a sentence */
	char line[MAX_LINE_LENGTH+1];	/* holds the next line in file*/
	char secline[MAX_LINE_LENGTH+1];	/* holds the line to be broken into tokens */
	char dataimage[dtimlen];		/* data image array */
	resetword(dataimage, dtimlen);
	
	
	char argument[17] = {'\0'};
	char afword[17];		/* arrtibute first word */
	char asword[17];		/* arrtibute second word */
	char csword[7];			/* command second word with \n\0 without ARE and funct, need to be added twice */
	char lword[5];			/* label first 4 bits R or E for base and offset values */
	char word[21];			/* word for translating binary to special base */
	char speword[15];		/* word for special base */

	int numline=0;			/* keep track on number of lines*/
	/*int i=0;			 index for parameter line*/
	int icount = 100;		/* keep record of instruction */
	int countcomma = 0;		/* count the number of commas in .data command */
	int iarg = 0;			/* index for arrguments */
	int arg;			/* holds the next arrgument */
	int j;				/* index for for in data/string command */
	int ascii;			/* parameter for string command */
	int linelen;			/* holds the current line length */
	int pos = (g->ic + 1);		/* index for counting the data image with g->ic */
	char posi[5] = "0000";		/* string for int pos */
	
	
	int labelflag = 0;		/* is on if the line is a part of a macro body */
	int labelfoundflag = 0;		/* flag for entry/extern check */
	int exterflag = 0;		/* flag for creating externals file */
	int entryflag = 0;		/* flag for creating entries file */
	int firstdataflag = 1;
	int eofflag = 1;		/* flag for eof*/
	int awordflag =0;		/* flag for different states in afword/asword in command to binary */


	struct LTNode *sectemp;		/* temp parameter for checking label names */
	
	sectemp = head;			/* set the pointer to the firrst node in list */
	
	/* ***************************************************************************************************************************** */
	
			/* ----- processing the input file ----- */

	resetword(dataimage, dtimlen);
			/* --- phase 1 in algorithem --- */	/* check for function in sidefunctions */
	int q = (g->ic) - 100;
	fprintf(fp, "%d %d\n", q, (g->dc));
	fprintf(fp, "0%d %s", icount, ":    ");

	while (eofflag)		/* ????? check end of file ????? */
	{
		labelflag = 0;
		resetword(line, MAX_LINE_LENGTH+1);
		resetword(secline, MAX_LINE_LENGTH+1);
		
		eofflag = getline(infp, line);		/* ??????? correct syntax ???????? */
		if ( (strlen(line)) == 0)
		{
			continue;
		}
		
	  	strcpy(secline, line);		/* copy line to secline */
	  	
	  	numline++;
	
	/* ***************************************************************************************************************************** */
	
	
			/* --- phase 2 in algorithem --- */
		
		if (strchr(line, ':') != NULL)		/* check if there is a label deffinition in the line */
		{
			token = strtok(secline, ":");
			token = strtok(NULL, ":");		/* remove the label */
			labelflag = 1;
		}
		
			/* --- phase 3 - extern - in algorithem --- */
		
		if (strstr(line, ".extern") != NULL)		/* check if the line is in format: .extern label */
		{
			exterflag = 1;
			continue;		/* skip .extern line */
		}
		
	/* ***************************************************************************************************************************** */

			/* ----- .data command ----- */
			
		if (strstr(line, ".data") != NULL)		
		{	
			if (firstdataflag)
			{
				inttostring(pos, posi);
				linelen = strlen(posi);
				strncat(dataimage, posi, linelen);
				resetposi(posi, 4);
			
				linelen = strlen(":    ");
				strncat(dataimage, ":    ", linelen);
				firstdataflag = 0;
			}
			
			if (labelflag)
			{
				/* ----- .data command with label ----- */
				token = strtok(token, " ");		/* get command */
				token = strtok(NULL, " ");		/* get arrtibutes */
			}
			else
			{
				/* ----- .data command without label ----- */
				token = strtok(secline, " ");		/* get command */
				token = strtok(NULL, " ");		/* get arrtibutes */
					
			}
			while (token[iarg] != '\n')
			{
				if (token[iarg] == ',')		/* count commas */
				{
					countcomma++;
				}
				iarg++;
			}
			iarg = 0;
			countcomma++;		/* num of arrguments */
			char *arti = (char *) malloc (strlen(token) +1 );
			strcpy(arti, token);
			token = strtok(token, ",");
			while (token != NULL)		/* process the arrguments */
			{
				arg = atoi(token);			/* shift the argument into int */
				dectobin(arg, argument);		/* decimel -> binary string */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "0100", 4);		/* set the 4 first digits */
				strncat(word, argument, 16);	/* set the argument in the next 16 digits */
				bintospecialbase(word,speword);
				strncat(dataimage, speword, 14);
				
				strncat(dataimage, "\n", 1);		/* add \n */
				
				pos++;
				if (pos != ((g->ic) + (g->dc) + 1))
				{
					inttostring(pos, posi);
					linelen = strlen(posi);
					strncat(dataimage, posi, linelen);
					resetposi(posi, 4);
			
					linelen = strlen(":    ");
					strncat(dataimage, ":    ", linelen);
				}
				
				resetword(argument, 17);
				
				countcomma--;
				token = strtok(NULL, ",");		/* get individual arrtibutes */
			}
			j = 0;
			continue;
		}
			
			
				/* ----- end of .data command ----- */
				
			
				/* ----- .string command ----- */
			
		if (strstr(line, ".string") != NULL)		/* .string command with label */
		{
			if (firstdataflag)
			{
				inttostring(pos, posi);
				linelen = strlen(posi);
				strncat(dataimage, posi, linelen);
				resetposi(posi, 4);
			
				linelen = strlen(":    ");
				strncat(dataimage, ":    ", linelen);
				firstdataflag = 0;
			}
			
			if (labelflag)
			{
				/* ----- .string command with label ----- */
					
				token = strtok(token, "\"");		/* get command */
				token = strtok(NULL, "\"");		/* get arrtibutes */
			}
			else
			{
				/* ----- .string command without label ----- */
				token = strtok(secline, "\"");		/* get command */
				token = strtok(NULL, "\"");		/* get arrtibutes */
					
			}
			int d = 0;	
			while (line[d] != '\"')
			{
				d++;		/* find first double qoutes */
			}
			d++;
			iarg = strlen(line)-2;
			for (j = d; j < iarg; j++)
			{
				ascii = line[j];		/* get ascii representation for char in int */
				dectobin(ascii, argument);	/* decimel -> binary string */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "0100", 4);		/* set the 4 first digits */
				strncat(word, argument, 16);	/* set the argument in the next 16 digits */
				bintospecialbase(word,speword);
				strncat(dataimage, speword, 14);
			
				strncat(dataimage, "\n", 1);		/* add \n */
				
				pos++;
				if (pos != ((g->ic) + (g->dc) + 1))
				{
					inttostring(pos, posi);
					linelen = strlen(posi);
					strncat(dataimage, posi, linelen);
					resetposi(posi, 4);
			
					linelen = strlen(":    ");
					strncat(dataimage, ":    ", linelen);
				}
				
				resetword(argument, 17);
					
			}
			continue;
				
				
		}
				/* ----- end of .string command ----- */
			 
				 /* ----- regular command with label ----- */
			
			
		if (labelflag)
		{
			/* ----- command with label ----- */
			token = strtok(token, " ");		/* get command */
			if ( strstr(token, "\n") != NULL)
			{
				token = strtok(token , "\n");	/* get command name without '\n' */
			}
		}
		else
		{
			/* ----- command without label ----- */
			token = strtok(secline, " ");		/* get command */
			if ( strstr(token, "\n") != NULL)
			{
				token = strtok(token , "\n");	/* get command name without '\n' */
			}
		}
			
		if ( ( strcmp( token, "rts" ) == 0) || ( strcmp( token, "stop" ) == 0) )
		{
			/* zero arrguments commands */
			
			if ( strcmp( token, "rts" ) == 0)
			{
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000100000000000000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				icount++;
				if (icount != (g->ic+1))
				{
					if (icount < 1000)
					{
						fprintf(fp, "0");
					}
					fprintf(fp, "%d %s", icount, ":    ");
				}
				continue;
				/*"01000100000000000000";*/
			}
				
			if ( strcmp( token, "stop" ) == 0)
			{
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01001000000000000000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				icount++;
				if (icount != (g->ic+1))
				{
					if (icount < 1000)
					{
						fprintf(fp, "0");
					}
					fprintf(fp, "%d %s", icount, ":    ");
				}
				continue;
				/*"01001000000000000000";*/
			}
				
		}
			
		if ( ( strcmp( token, "mov" ) == 0) || ( strcmp( token, "cmp" ) == 0) || ( strcmp( token, "add" ) == 0) || ( strcmp( token, "sub" ) == 0) || ( strcmp( token, "lea" ) == 0) )
		{
			/* two arrguments commands */
			
				
			if ( strcmp( token, "mov" ) == 0)
			{
					
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000000001", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}	
				fprintf(fp, "%d %s", icount, ":    ");
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000", 8);
				/*fprintf(fp, "%s","01000000");*/
				/*opcode = "01000000000000000001";
				funct = "01000000";*/
					
					
			}
				
			if ( strcmp( token, "cmp" ) == 0)
			{
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000000010", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				icount++;	
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000", 8);
				/*fprintf(fp, "%s","01000000");*/
				/*opcode = "01000000000000000010";
				funct = "01000000";*/
					
					
			}
				
			if ( strcmp( token, "add" ) == 0)
			{
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000000100", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				icount++;	
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01001010", 8);
				/*fprintf(fp, "%s","01001010");*/
				/*opcode = "01000000000000000100";
				funct = "01001010";*/
					
			}
				
			if ( strcmp( token, "sub" ) == 0)
			{
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000000100", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01001011", 8);
				/*fprintf(fp, "%s","01001011");*/
				/*opcode = "01000000000000000100";
				funct = "01001011";*/
					
					
			}
				
			if ( strcmp( token, "lea" ) == 0)
			{
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000010000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000", 8);
				/*fprintf(fp, "%s","01000000");*/
				/*opcode = "01000000000000010000";
				funct = "01000000";*/
					
					
			}
			token = strtok(NULL, " ");		/* get the arrtibutes */
			
			char st[strlen(token)+1];
			strcpy(st, token);			/* copy the arrtibutes to st */
			token = strtok(token, ",");		/* get the first arrtibute */
			int e;
			for ( e = 0; e < 4; e++)
			{
				resetword(afword, 17);
				resetword(asword, 17);
				resetword(csword, 7);
				resetword(lword, 5);
					
				/*arrtobin(char *ar, char *fw, char *sw, char *rw, char *lw, struct LTNode *head, int numline)*/
					
				if ( e == 0)		/* csword for first arrgument */
				{
					if ( strstr(token, "\n") != NULL)
					{
						token = strtok(token , "\n");	/* get command name without '\n' */
					}
					arrtobin(token, afword, asword, csword, lword, head, icount);
					strncat(word, csword, 6);
				}
					
				if ( e == 1)		/* csword for second arrgument */
				{
					strcpy(token, st);
					token = strtok(NULL , ",");	/* get the second arrtibute */
					if ( strstr(token, "\n") != NULL)
					{
						token = strtok(token , "\n");	/* get command name without '\n' */
					}
					arrtobin(token, afword, asword, csword, lword, head, icount);
					strncat(word, csword, 6);
					bintospecialbase(word,speword);
				
					fprintf(fp, "%s\n", speword);
					icount++;
					if (icount != (g->ic+1))
					{
						if (icount < 1000)
						{
							fprintf(fp, "0");
						}
						fprintf(fp, "%d %s", icount, ":    ");
					}
				}
					
				if ( e == 2)		/* other words for first arrgument */
				{
					strcpy(token, st);
					token = strtok(token, ",");		/* get the first arrtibute */
					
				}
					
				if ( e == 3)		/* other words for second arrgument */
				{
					strcpy(token, st);
					token = strtok(NULL , ",");	/* get the second arrtibute */
				}
					
				if ( e > 1)
				{
					if ( strstr(token, "\n") != NULL)
					{
						token = strtok(token , "\n");	/* get command name without '\n' */
					}
					resetposi(afword, 16);
					resetposi(asword, 16);
					awordflag = arrtobin(token, afword, asword, csword, lword, head, icount);
					if ( awordflag > 0)
					{
						resetword(word, 21);
						resetword(speword, 15);
						strncat(word, lword, 4);		/* set the  first word */
						strncat(word, afword, 16);
						bintospecialbase(word,speword);
				
						fprintf(fp, "%s\n", speword);
						
						icount++;
						if (icount != (g->ic+1))
						{
							if (icount < 1000)
							{
								fprintf(fp, "0");
							}
							fprintf(fp, "%d %s", icount, ":    ");
						}
							
						if (awordflag == 2)
						{
							resetword(word, 21);
							resetword(speword, 15);
							strncat(word, lword, 4);		/* set the  first word */
							strncat(word, asword, 16);
							bintospecialbase(word,speword);
				
							fprintf(fp, "%s\n", speword);
							
							icount++;
							if (icount != (g->ic+1))
							{
								if (icount < 1000)
								{
									fprintf(fp, "0");
								}
								fprintf(fp, "%d %s", icount, ":    ");
							}
						}
					}
				}
			}
			continue;
				
		}	/* end of two arrguments commands */
			
		if ( ( strcmp( token, "clr" ) == 0) || ( strcmp( token, "not" ) == 0) || ( strcmp( token, "inc" ) == 0) || ( strcmp( token, "dec" ) == 0) || ( strcmp( token, "jmp" ) == 0) || ( strcmp( token, "bne" ) == 0) || ( strcmp( token, "jsr" ) == 0) || ( strcmp( token, "red" ) == 0) || ( strcmp( token, "prn" ) == 0) )
		{
			/* one arrgument commands */
			
			if ( strcmp( token, "clr" ) == 0)
			{
				token = strtok(NULL, " ");		/* get the arrtibutes */
				token = strtok(token, ",");		/* get the first arrtibute */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000100000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				/*fprintf(fp, "%s","01000000000000100000\n");*/
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01001010000000", 14);
				/*fprintf(fp, "%s","01001010000000");*/
				/*opcode = "01000000000000100000";
				funct = "01001010";*/
			}
				
			if ( strcmp( token, "not" ) == 0)
			{
				token = strtok(NULL, " ");		/* get the arrtibutes */
				token = strtok(token, ",");		/* get the first arrtibute */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000100000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				/*fprintf(fp, "%s","01000000000000100000\n");*/
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01001011000000", 14);
				/*fprintf(fp, "%s","01001011000000");*/
				/*opcode = "01000000000000100000";
				funct = "01001011";*/
			}
				
			if ( strcmp( token, "inc" ) == 0)
			{
				token = strtok(NULL, " ");		/* get the arrtibutes */
				token = strtok(token, ",");		/* get the first arrtibute */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000100000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				/*fprintf(fp, "%s","01000000000000100000\n");*/
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01001100000000", 14);
				/*fprintf(fp, "%s","01001100000000");*/
				/*opcode = "01000000000000100000";
				funct = "01001100";*/
			}
				
			if ( strcmp( token, "dec" ) == 0)
			{
				token = strtok(NULL, " ");		/* get the arrtibutes */
				token = strtok(token, ",");		/* get the first arrtibute */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000100000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				/*fprintf(fp, "%s","01000000000000100000\n");*/
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01001101000000", 14);
				/*fprintf(fp, "%s","01001101000000");*/
				/*opcode = "01000000000000100000";
				funct = "01001101";*/
			}
				
			if ( strcmp( token, "jmp" ) == 0)
			{
				token = strtok(NULL, " ");		/* get the arrtibutes */
				token = strtok(token, ",");		/* get the first arrtibute */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000001000000000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				/*fprintf(fp, "%s","01000000001000000000\n");*/
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01001010000000", 14);
				/*fprintf(fp, "%s","01001010000000");*/
				/*opcode = "01000000001000000000";
				funct = "01001010";*/
			}
				
			if ( strcmp( token, "bne" ) == 0)
			{
				
				token = strtok(NULL, " ");		/* get the arrtibutes */
				token = strtok(token, ",");		/* get the first arrtibute */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000001000000000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				/*fprintf(fp, "%s","01000000001000000000\n");*/
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01001011000000", 14);
				/*fprintf(fp, "%s","01001011000000");*/
				/*opcode = "01000000001000000000";
				funct = "01001011";*/
			}
				
			if ( strcmp( token, "jsr" ) == 0)
			{
				token = strtok(NULL, " ");		/* get the arrtibutes */
				token = strtok(token, ",");		/* get the first arrtibute */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000001000000000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				/*fprintf(fp, "%s","01000000001000000000\n");*/
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01001100000000", 14);
				/*fprintf(fp, "%s","01001100000000");*/
				/*opcode = "01000000001000000000";
				funct = "01001100";*/
			}
				
			if ( strcmp( token, "red" ) == 0)
			{
				token = strtok(NULL, " ");		/* get the arrtibutes */
				token = strtok(token, ",");		/* get the first arrtibute */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000001000000000000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				/*fprintf(fp, "%s","01000001000000000000\n");*/
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000", 14);
				/*fprintf(fp, "%s","01000000000000");*/
				/*opcode = "01000001000000000000";
				funct = "01000000";*/
			}
				
			if ( strcmp( token, "prn" ) == 0)
			{
				token = strtok(NULL, " ");		/* get the arrtibutes */
				token = strtok(token, ",");		/* get the first arrtibute */
				
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000010000000000000", 20);		/* set the  first word */
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				
				/*fprintf(fp, "%s","01000010000000000000\n");*/
				icount++;
				if (icount < 1000)
				{
					fprintf(fp, "0");
				}
				fprintf(fp, "%d %s", icount, ":    ");
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, "01000000000000", 14);		/* set the  first word */
				
				/*fprintf(fp, "%s","01000000000000");*/
				/*opcode = "01000010000000000000";
				funct = "01000000";*/
			}
				
			resetword(afword, 17);
			resetword(asword, 17);
			resetword(csword, 7);
			resetword(lword, 5);
			
			if ( strstr(token, "\n") != NULL)
			{
				token = strtok(token , "\n");	/* get command name without '\n' */
			}
			
			awordflag = arrtobin(token, afword, asword, csword, lword, head, icount);
			strncat(word, csword, 6);
			bintospecialbase(word,speword);
				
			fprintf(fp, "%s\n", speword);
			icount++;
			if (icount < 1000)
			{
				fprintf(fp, "0");
			}
			fprintf(fp, "%d %s", icount, ":    ");
			if ( awordflag > 0)
			{
				resetword(word, 21);
				resetword(speword, 15);
				strncat(word, lword, 4);		/* set the  first word */
				strncat(word, afword, 16);
				bintospecialbase(word,speword);
				
				fprintf(fp, "%s\n", speword);
				icount++;
				if (icount != (g->ic+1))
				{
					if (icount < 1000)
					{
						fprintf(fp, "0");
					}
					fprintf(fp, "%d %s", icount, ":    ");
				}
							
				if ( awordflag == 2)
				{
					resetword(word, 21);
					resetword(speword, 15);
					strncat(word, lword, 4);		/* set the  first word */
					strncat(word, asword, 16);
					bintospecialbase(word,speword);
				
					fprintf(fp, "%s\n", speword);
					
					icount++;
					if (icount != (g->ic+1))
					{
						if (icount < 1000)
						{
							fprintf(fp, "0");
						}
						fprintf(fp, "%d %s", icount, ":    ");
					}
				}
			}
		}	/* end of one arrgument commands */

			


	/* ***************************************************************************************************************************** */
		
		
		
		if ( (strstr(line, ".entry") != NULL) )		/* check if the line is in format: .entry label */
		{
			/* find the label and update entry arrtibute */
			strcpy(secline, line);		/* copy line to secline */
			entryflag = 1;
			token = strtok(secline, " ");		/* get command */
			token = strtok(NULL , " ");		/* get label name */
			if ( strstr(token, "\n") != NULL)
			{
				token = strtok(token , "\n");	/* get command name without '\n' */
			}
			while (sectemp != NULL)
			{
				if ( strcmp(token, sectemp->name ) == 0)	/* find the label in the list */
				{
					labelfoundflag = 1;
					sectemp->ent = 1;
					break;
				}
				sectemp = sectemp->next;
			}	/* ----- end of while -----*/
			sectemp = head;			/* reset checking pointer to start of label list */
			
			if (labelfoundflag)
			{
				labelfoundflag = 0;
				continue;
			}
			else
			{
				g->errorflag = 1;
				printf("Label - %s -  isn't declared in the file. Line number %d .\n", token, numline);
				labelfoundflag = 0;
				continue;
			}
			
			
		}		/*  end of if ( (strstr(line, ".entry") != NULL) ) */
		
		
		
	}	/* end of while EOF */
		/* append data image */
	
	fprintf(fp, "%s", "\n\n\n");
	fprintf(fp, "%s", dataimage);
	
	
	if (exterflag)
	{
		/* !!!!!!!!!!!!!!!!!!!!!!!! FILE name.entries !!!!!!!!!!!!!!!!!!!!!!!! */
		FILE *ex;
		sectemp = head;
		char *exname = (char *) malloc (strlen(path) +5 );
	
		strcpy(exname,path);
		strcat(exname,".ext");
		ex = fopen(exname,"w");	/* Try to open file */
		while (sectemp != NULL)
		{
			if (sectemp->ext == 1)
			{
				fprintf( ex, "%s %s", sectemp->name, " ");	/* ?????? */
				fprintf( ex, "%s %d %s", "BASE ", sectemp->base, "\n");	/* ?????? */
				fprintf( ex, "%s %s", sectemp->name, " ");	/* ?????? */
				fprintf( ex, "%s %d %s", "OFFSET ", sectemp->offset, "\n\n");	/* ?????? */
			}
			sectemp = sectemp->next;
		}
		free(exname);
		fclose(ex);
	}
	
	if (entryflag)
	{
		/* !!!!!!!!!!!!!!!!!!!!!!!! FILE name.externals !!!!!!!!!!!!!!!!!!!!!!!! */
		FILE *en;
		sectemp = head;
		char *enname = (char *) malloc (strlen(path) +5 );
	
		strcpy(enname,path);
		strcat(enname,".ent");
		en = fopen(enname,"w");	/* Try to open file */
		while (sectemp != NULL)
		{
			if (sectemp->ent == 1)
			{
				fprintf( en, "%s %s", sectemp->name, " , ");
				fprintf( en, "%d %s", sectemp->base, " , ");
				fprintf( en, "%d %s", sectemp->offset, "\n");
			}
			sectemp = sectemp->next;
			
		}
		free(enname);
		fclose(en);
	}
		
	free(newname);
	
	fclose(infp);
	fclose(fp);
	return 0;
	
	/*!!!!!!!!!!!!!!!!!!!!!!!!!freeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee !!!!!!!!!!!!!!!! */	
		
		
}		
