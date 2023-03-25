#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include "preassmbler.h"
#include "structs.h"
#include "sidefunctions.h"




int preassmbler( char *path, struct global *g)
{
	FILE *fp;
	char *newname = (char *) malloc (strlen(path) +4 );
	strcpy(newname,path);
	strcat(newname,".as");
	fp = fopen(newname,"r");	/* Try to open file */
	if (fp == NULL)  	/* If file does not exists */
	{
		printf("File %s not found. \n", path);
		return 0;
	}


				/* ----- function that changes the ending of string ----- */
						/* ----- creating a file.as ----- */
	
	resetword(newname, strlen(newname));
	FILE *outfp;
	strcpy(newname,path);
	strcat(newname,".am");
	outfp = fopen( newname, "w");

	char InsComList[21][7] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop", "macro", "data", "string", "entry", "extern " };
	char *token;		/* pointer for representing each word in a sentence */
	char line[MAX_LINE_LENGTH+1];	/* holds the next line in file*/
	char secline[MAX_LINE_LENGTH+1];	/* holds the next line in file for strtok*/
	char tav;				/* next char in line*/		/* was without * */
	char *ibuf;				/* pointer to cell in buffer */
	char buffer[256] = {'\0'};			/* buffer for macro body */

	int sumbuf = 0;			/* kepps track of next position to enter the next line */
	int numline=1;			/* keep track on number of lines*/
	int spaceflag =0;	/* flag for 2 adjecnt whitespace characters */
	int i=0;		/* index for parameter line*/
	int macroflag = 0;	/* is on if the line is a part of a macro body */
	int firstmacro = 1;	/* this is the first macro */
	int doublenameflag = 0;	/* flag for indicating that the macro name is already in use */
	int macronameflag = 0;	/* flag for skiping a macro with an invalid name */
	int bodyreplacedflag = 0;	/* flag for skipping to next line after replacing the macro name with it's body */
	int eofflag = 1;		/* flag for eof*/
	
	int j;

	struct MNode *head = NULL;
	struct MNode *temp = NULL;	/* temp parameter for chaining the list */
	struct MNode *sectemp = NULL;	/* temp parameter for checking macro names */
	

			/* ----- creating a file.as ----- */




			/* ----- processing the input file ----- */


	while (eofflag)		/* check end of file */
	{
		resetword(line, MAX_LINE_LENGTH+1);
		tav = '\0';
		while ((tav != '\n') && (tav != EOF )) 		/*get the next line in file*/
	   	{
			if ( i > 80)	/*check line length error*/
			{
				g->errorflag = 1;
				printf("Line number %d is over 81 characters" , numline );
				break;
			}
	        	tav = fgetc(fp);
	        	if (tav == EOF)
	        	{
	        		eofflag = 0;
	        		continue;
	        	}
	        	if ((i == 0) && ( (tav ==' ') || (tav == '\t') || (tav == '\n') ) ) 
	        	{
	        		continue;
	        	}	/* skip all whitespace in the start of the line */
	        	
			if ( (tav ==' ') || (tav == '\t') )		/* ?????? should I add more whitespaces keys ????? */
			{
				if (spaceflag)
				{
					continue;
				}	/*check for 2 adjecnt whitespace*/
				spaceflag = 1;
				line[i] = ' ';	/*no tabs allowed ?????*/
				i++;
			}
			else
			{
				spaceflag = 0;
				line[i] = tav;
				i++;
				
			}
			
	  	}
	  	
	  	line[i] = '\0';
	  	if (i == 0)
	  	{
	  		continue;
	  	}
	  	strcpy(secline, line);
	  		/*-----end second while-----*/
		i = 0;
		token = strtok(secline , " ");	/*check first word */
		if ((strcmp(token, "endm\n") == 0) && !(macronameflag))		/* end of macro body  */
		{
			macroflag = 0;		/* reset flag after the end of the macro */
			doublenameflag = 0;	/* reset flag after the end of the macro */
			macronameflag = 0;	/* reset flag after the end of the macro */
			temp->body =(char *) malloc (strlen(buffer) +1 );
			strcpy(temp->body, buffer);
			sumbuf = 0;
			memset(buffer, '\0', sizeof(buffer));
			continue;
		}
		
			/* ----- insert the next line in macro to it's body -----*/
		
		
		if (macroflag && !(macronameflag))		/* ----- insert the next line in macro to it's body ----- */
		{
			ibuf = &(buffer[sumbuf]);
			strcpy(ibuf, line);		/* ????? maybe use a pointer to buffer  ?????? */
			sumbuf += strlen(line);		/* update the next position for the next line */
			continue;
		}

	



			/* ----- check macro name in file and subsetuting it with it's body -----*/


		while (!(firstmacro) && (sectemp != NULL))	/* ???? condition syntax is good ?????? */
		{
			if ( strcmp(token, sectemp->name) == 0 )
			{
				fputs(sectemp->body, outfp);		/* insert the macro's body instead of the original line */

				bodyreplacedflag = 1;
				break;
			}
			sectemp = sectemp->next;
		}	/* ----- end of while -----*/
		if (!(firstmacro))
		{
			sectemp = head;
		}
		if (bodyreplacedflag)
		{
			bodyreplacedflag = 0;		/* reset checking poineter to start ot label list */
			continue;
		}
	
			/* ----- check if there is a defintion of macro ----- */

		if (strcmp(token, "macro") == 0)
		{
			token = strtok(NULL , " ");	/* ?????? get the name of the macro ??????*/
			if (strlen(token) > 31)		/* --- macro name length error --- */
			{
				g->errorflag = 1;
				printf("In line number %d the macro name is over 31 characters" , numline );
				macronameflag = 1;
			}


			for (j=0; j < 21; ++j)	/* --- instruction/command macro name error --- */
			{
				if ( strcmp(token, InsComList[j]) == 0 )
				{
					g->errorflag = 1;
					printf("In line number %d the macro name is the same as an instruction/command." , numline );
					macronameflag = 1;
					break;
				}
			}


			if (firstmacro)
			{
				if (macronameflag)
				{
					continue;
				}	/* skip the macro if invalid name */
				head = malloc(sizeof( struct MNode));
				head->next = NULL;
				strcpy(head->name ,token);
				temp = head;		/* set the poineter to head */
				macroflag = 1;		/* next line is part of the macro */
				sectemp = head;		/* set checking pointer */
				firstmacro = 0;		/* first macro is done */
				continue;
			}		/* ????? what about capital letters ???? */
			else
			{
	
				/* ----- check if macro name already exist ----- */
	
				
				if (macronameflag)
				{
					continue;
				}	/* skip the macro if invalid name */
				while (sectemp != NULL)
				{
					if ( strcmp(token, sectemp->name ) == 0)
						{
						g->errorflag = 1;
						printf ( "Macro name - %s - already exist" , token);
						doublenameflag = 1;
						break;
						}
					sectemp = sectemp->next;
				}	/* ----- end of while -----*/
				sectemp = head;			/* reset checking poineter to start of label list */
	
				if (doublenameflag)		/* skip line if it is in body of macro name already been used */
				{
					continue;
				}
				struct MNode *a = malloc(sizeof( struct MNode));
				strcpy(a->name, token);
				a->next = NULL;
				temp->next = a;
				temp = a;		/* set the poineter to the last node */
				macroflag = 1;		/* next line is part of the macro */
				continue;
			}
			numline++;
		}
		fprintf(outfp, "%s", line);		/* insert the line of the original line to the ".as" file */

		/* enter the line to a file with ending .as ?????*/
	
	}	/*-----end first while-----*/
	
	fclose(fp);	/* Close file */
  	fclose(outfp);	/* Close file */
  	sectemp = head;
	while (sectemp != NULL)
	{
		temp = sectemp;
		sectemp = sectemp->next;
		free(temp);
	}
	free(newname);
  	return 0;	


}





