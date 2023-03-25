#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include "firstwalk.h"
#include "structs.h"
#include "sidefunctions.h"
#include "secondwalk.h"



void firstWalk(char *path, struct global *g)
{
	
	/* ***************************************************************************************************************************** */
	
	FILE *fp;
	
	char *newname = (char *) malloc (strlen(path) +4 );
	
	strcpy(newname,path);
	strcat(newname,".am");
	fp = fopen(newname,"r");	/* Try to open file */

	if (fp == NULL)  	/* If file does not exists */
	{
		return;
	}

	/* ***************************************************************************************************************************** */



		
	char InsComList[21][7] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop", "macro", "data", "string", "entry", "extern " };
	char *token;				/* pointer for representing each word in a sentence */
	char line[MAX_LINE_LENGTH+1];	/* holds the next line in file*/
	char secline[MAX_LINE_LENGTH+1];	/* holds the line to be broken into tokens */

	
	int numline=0;			/* keep track on number of lines*/
	int j = 1;			/* index for check label name error */
	int k = 0;			/* index for label saved word check */
	int idata = 0;			/* index for checking how much word to add for a data instruction */
	int count = 0;			/* count the number of " in .string command */
	
	
	int labelflag = 0;		/* is on if the line is a part of a macro body */
	int firstlabel = 1;		/* this is the first label */
	int doublenameflag = 0;		/* flag for indicating a name already been used */
	int extflag = 0;		/* flag for format: .extern label */
	int commfoundflag = 0;		 /*flag for compering command name */
	int eofflag = 1;		/* flag for eof*/
	
	
	struct LTNode *temp = NULL;		/* temp parameter for chaining the list */
	struct LTNode *sectemp = NULL;		/* temp parameter for checking label names */
	struct LTNode *head = NULL;

	/* ***************************************************************************************************************************** */
	
			/* ----- processing the input file ----- */

			
			/* --- phase 2 in algorithem --- */
			

	while (eofflag)		/* ????? check end of file ????? */
	{
		resetword(line, MAX_LINE_LENGTH+1);
		resetword(secline, MAX_LINE_LENGTH+1);
		
		eofflag = getline(fp, line);
		if ( (strlen(line)) == 0)
		{
			continue;
		}
	  	strcpy(secline, line);		/* copy line to secline */
	  		
	  	numline++;
		

	/* ***************************************************************************************************************************** */

			/* ----- check existence of label and valid name ----- */
			
				/* ----- phase 3-4 in algorithem ----- */
		
		if (strchr(line, ':') != NULL)		/* check if there is a label deffinition in the line */
		{
			labelflag = 1;
			token = strtok(secline , ":");	/* get label name */
			
		
		/* ----- check for non alpha-numric char between the label name and the colon sign ----- 
		
			if ( ( ( isalpha(token[strlen(token)])) == 0 ) || ((isdigit(token[strlen(token)])) == 0) )
			{
				g->errorflag = 1;
				labelflag = 0;		*//* don't insert the invalid label */
							
				/*printf ( "label name - %s - isn't adjacent to colon. Line number %d .\n" , token, numline);
			}
			*/
			/* ----- check if the first char is a non alpha char  ----- */
			
			if (( isalpha(token[0])) == 0)
			{
				g->errorflag = 1;
				labelflag = 0;		/* don't insert the invalid label */
							
				printf ( "label name - %s - has to start with a lower/upper case alpha charecter. Line number %d .\n" , token, numline);
			}
		
			/* ----- check if there is a non alpha-numric char in the label name and name length error ----- */
			
			while ( token[j] != '\0' )
			{
				if ( ( ( isalpha(token[j])) == 0 ) && ((isdigit(token[j])) == 0) )
				{
					g->errorflag = 1;
					labelflag = 0;		/* don't insert the invalid label */
							
					printf ( "label name - %s - has a non alpha-numric charecter. Line number %d .\n" , token, numline);
					break;
				}
				j++;
				if ( j > 31)
				{
					g->errorflag = 1;
					labelflag = 0;		/* don't insert the invalid label */
						
					printf ( "label name - %s - has more then 31 charecters. Line number %d .\n" , token, numline);
					break;
				}
			}
			j = 1;		/* reset the index for name error */
			
			
			
			/* !!!!!!!!!!!!!!!! need to add a check for saved words !!!!!!!!!!!! */
			
			for (k=0; k < 21; ++k)
			{
				if ( strcmp(token, InsComList[k] ) == 0)
					{
						g->errorflag = 1;
						labelflag = 0;		/* don't insert the invalid label */
						
						printf ( "label name - %s - can't be a saved word. Line number %d .\n" , token, numline);
						break;
					}
			}
			
		}	/* end of if (strchr(line, ":") != NULL) */

				/* ----- end of phase 3-4 in algorithem ----- */
				
	/* ***************************************************************************************************************************** */


			/* ----- first create a struct for label then update it with what we know ----- */
			
				/* ----- phase 10 and prepartion for phase 11 in algorithem ----- */
		
		if (strstr(line, ".extern") != NULL)		/* check if the line is in format: .extern label */
		{
			extflag = 1;
			token = strtok(secline , ".extern ");	/* get label name */
			while (sectemp != NULL)			/* check if the label already been defined */
			{
				if ( strcmp(token, sectemp->name ) == 0)
				{
					g->errorflag = 1;
					extflag = 0;
					printf ( "label name - %s - already exist so it can't be external. Line number %d .\n" , token, numline);
					break;
				}
				sectemp = sectemp->next;
			}
							/* ----- end of while -----*/
			if (!(firstlabel))
			{
				sectemp = head;			/* reset checking pointer to start of label list */
			}
		}		/* end of if (strstr(line, ".extern") != NULL) */
		
		if ( (labelflag) || (extflag) )
		{
			if (firstlabel)
			{
				if ( extflag )
				{
					head = malloc(sizeof( struct LTNode));
					head->ent = 0;
					head->code = 0;
					head->data = 0;
					head->next = NULL;
					head->val = 0;
					head->base = 0;
					head->offset = 0;
					head->ext = 1;
					if ( strstr(token, "\n") != NULL)
					{
						token = strtok(token , "\n");	/* get command name without '\n' */
					}
					strcpy(head->name ,token);
					temp = head;		/* set the poineter to head */
					sectemp = head;		/* set checking pointer */
					firstlabel = 0;		/* first label is done */
					labelflag = 0;
					extflag = 0;
					continue;
				}
				else
				{
					head = malloc(sizeof( struct LTNode));
					head->ext = 0;
					head->ent = 0;
					head->code = 0;
					head->data = 0;
					head->next = NULL;
					if ( strstr(token, "\n") != NULL)
					{
						token = strtok(token , "\n");	/* get command name without '\n' */
					}
					strcpy(head->name ,token);
					temp = head;		/* set the poineter to head */
					sectemp = head;		/* set checking pointer */
					firstlabel = 0;		/* first label is done */
				}
			}
			else
			{
				/* ----- check if label name already exist ----- */
		
				while (sectemp != NULL)
				{
					if ( strcmp(token, sectemp->name ) == 0)
					{
						g->errorflag = 1;
						doublenameflag = 1;	/* don't insert the invalid label */
						
						printf ( "label name - %s - already exist. Line number %d .\n" , token, numline);
						break;
					}
					sectemp = sectemp->next;
				}	/* ----- end of while -----*/
				sectemp = head;			/* reset checking pointer to start of label list */
				if (doublenameflag == 0)
				{

					if ( extflag )
					{
						struct LTNode *a = malloc(sizeof( struct LTNode));
						if ( strstr(token, "\n") != NULL)
						{
							token = strtok(token , "\n");	/* get command name without '\n' */
						}
						strcpy(a->name, token);
						a->val = 0;
						a->base = 0;
						a->offset = 0;
						a->ext = 1;
						a->ent = 0;
						a->code = 0;
						a->data = 0;
						a->next = NULL;
						temp->next = a;
						temp = a;		/* set the pointer to the last node */
						labelflag = 0;
						extflag = 0;
						continue;
					}
					else
					{
						struct LTNode *a = malloc(sizeof( struct LTNode));
						a->val = 0;
						a->base = 0;
						a->offset = 0;
						a->ext = 0;
						a->ent = 0;
						a->code = 0;
						a->data = 0;
						a->next = NULL;
						if ( strstr(token, "\n") != NULL)
						{
							token = strtok(token , "\n");	/* get command name without '\n' */
						}
						strcpy(a->name, token);
						temp->next = a;
						temp = a;		/* set the pointer to the last node */
					}
				}
				
			
			}	/* end of if (firstlabel) */

			
	/* ***************************************************************************************************************************** */

				
				/* ----- phase 6 in algorithem ----- */

			
			if(doublenameflag == 0)
			{			
				if (strstr(line, ".data") != NULL)		/* .data command with label */
				{
					while (line[idata] != '\n')
					{
						if ( line[idata] == ',' )
						{
							count++;
						}
						idata++;
					}
					count++;
					int x =count;
			
					token = strtok(NULL , ",");		/* get first parmeter */
					while ( token != NULL )
					{
						(g->dc)++;
						count--;
						token = strtok(NULL , ",");
					}
					if (count == 0)
					{
						g->dc = (g->dc) - x;
						temp->data = 1;
						temp->val = g->dc;		/* set the address of the label */
						temp->offset = ((g->dc) % 16);			/* set the offset value */
						temp->base = ((temp->val) - (temp->offset));	/* set the base value */
						idata = 0;
						g->dc = (g->dc) + x;
						labelflag = 0;
						continue;
					}
					else
					{
						g->errorflag = 1;
						printf("Insufficiant commas in .data command. Line number %d .\n" , numline);
						g->dc = (g->dc) - x;
						idata = 0;
						count = 0;
						labelflag = 0;
						continue;
					}
				}
				
				
				if (strstr(line, ".string") != NULL)		/* .string command with label */
				{
					
					while (line[idata] != '\n')
					{
						if ( line[idata] == '\"' )
						{
							count++;
						}
						idata++;
					}
					if ( count >= 2)
					{
						token = strtok(NULL , "\"");		/* token = .string */
						token = strtok(NULL , "\"");		/* token = arguments */
						if (strlen(token) == 0)
						{
							g->errorflag = 1;
							printf ( ".string command needs to have two arguments. Line number %d .\n", numline);		
						}
						else		/* set the label */
						{
							temp->data = 1;
							temp->val = g->dc;		/* set the address of the label */
							temp->offset = ((g->dc) % 16);			/* set the offset value */
							temp->base = ((temp->val) - (temp->offset));	/* set the base value */
							int d = 0;	
							while (line[d] != '\"')
							{
								d++;		/* find first double qoutes */
							}
							d++;
							int m = strlen(line)-2;
							g->dc = ((g->dc) + (m - d));
						}
					}
					else		/* error no 2 " */
					{
						g->errorflag = 1;
				
						
						printf ( ".string command needs to have at least two quotation marks. Line number %d .\n" , numline);
					}
			
			
					idata = 0;
					count = 0;
					labelflag = 0;
			
					continue;
				}
					
				/* ----- end of phase 6 in algorithem ----- */
				
				
			}		/* end of if doublenameflag */
			
			
			
		}		/* ----- end of labelflag if ----- */
		
	/* ***************************************************************************************************************************** */
					
		
				/* ----- phase 7 in algorithem ----- */
			
			/* ----- process how many words to add to dc ----- */
		
		if (strstr(line, ".data") != NULL)		/* .data command without label */
		{
			while (line[idata] != '\n')
			{
				if ( line[idata] == ',' )
				{
					count++;
				}
				idata++;
			}
			count++;
			int x =count;
			if (x == 1)
			{
				token = strtok(secline , " ");
				token = strtok(NULL , " ");		/* get first parmeter */
			}
			else
			{
				token = strtok(secline , " ");
				token = strtok(NULL , " ");
				token = strtok(token , ",");		/* get first parmeter */
			}
			while ( token != NULL )
			{
				(g->dc)++;
				count--;
				if (x == 1)
				{
					token = strtok(NULL , " ");
				}	
				else
				{
					token = strtok(NULL , ",");
				}
			}
			if (count == 0)
			{
				idata = 0;
				continue;
			}
			else
			{
				g->errorflag = 1;
				printf("Insufficiant commas in .data command. Line number %d .\n" , numline);
				g->dc = (g->dc) - x;
				idata = 0;
				count = 0;
				continue;
			}
		}
		
		if (strstr(line, ".string") != NULL)		/* .string command without label */
		{
			while (line[idata] != '\n')
			{
				if ( line[idata] == '\"' )
				{
					count++;
				}
				idata++;
			}
			if ( count >= 2)
			{
				token = strtok(secline , "\"");		/* token = .string */
				token = strtok(NULL , "\"");		/* token = arguments */
				if (strlen(token) == 0)
				{
					g->errorflag = 1;
					printf ( ".string command need to have two arguments. Line number %d .\n" , numline);
				}
				else		/* error no 2 " */
				{
					int d = 0;	
					while (line[d] != '\"')
					{
						d++;		/* find first double qoutes */
					}
					d++;
					int m = strlen(line)-2;
					g->dc = ((g->dc) + (m - d));
				}
			}
			else		/* error no 2 " */
			{
				g->errorflag = 1;
				
						
				printf ( ".string command needs to have at least two quotation marks. Line number %d .\n", numline);
			}
			
			
			idata = 0;
			count = 0;
			
			
			continue;
		}
		
		
			/* ----- end of phase 7 in algorithem ----- */
			
		
			/* ----- phase 9 in algorithem ----- */
		
		if ( (strstr(line, ".entry") != NULL) )		/* check if the line is in format: .entry label */
		{
			continue;		/* skip line */
		}
			/* ----- end of phase 9 in algorithem ----- */
		
	/* ***************************************************************************************************************************** */
		
		/* ------ phase 11 in algorithem ----- */
		
		if ( labelflag )
		{
			temp->code = 1;
			temp->val = g->ic;		/* set the address of the label */
			temp->offset = ((g->ic) % 16);			/* set the offset value */
			temp->base = ((temp->val) - (temp->offset));	/* set the base value */
			
			token = strtok(NULL , " ");	/* get command name */
			labelflag = 0;
		}
		else
		{
			token = strtok(secline , " ");	/* get command name */
		}
		
			/* ----- phase 12 in algorithem ----- */
		
		if ( strstr(token, "\n") != NULL)
		{
			token = strtok(token , "\n");	/* get command name without '\n' */
		}
			
		for ( k=0; k < 16; ++k )		/* compare command line with command list */
		{
			if ( strcmp(token, InsComList[k] ) == 0)
			{
				commfoundflag = 1;
				break;
			}
		}
		if ( commfoundflag )
		{
			if ( ( strcmp( token, "rts" ) == 0) || ( strcmp( token, "stop" ) == 0) )
			{
				(g->ic)++;
				continue;
			}
			else			/* more than 1 word */
			{
				if ( ( strcmp( token, "mov" ) == 0) || ( strcmp( token, "cmp" ) == 0) || ( strcmp( token, "add" ) == 0) || ( strcmp( token, "sub" ) == 0) || ( strcmp( token, "lea" ) == 0) )
				{
					/* 2 arguments command */
					(g->ic)++;
					(g->ic)++;
					int f;
					for ( f = 0; f < 2; f++)
					{
						token = strtok(NULL , ",");	/* get argument */
						if ( strstr(token,"[") != NULL )		/* check for index methode */
						{
							int h = 0;
							while (token[h] != '[')
							{
								h++;
							}
							h++;
							if (token[h] != 'r')
							{
								g->errorflag = 1;
								printf ( "Register inside index should have the letter 'r' - %s . Line number %d .\n" , token, numline);
							}
							h++;
							h++;
							if (isdigit(token[h]) == 0)
							{
								g->errorflag = 1;
								printf ( "Register inside index should have a number between 10 - 15 - %s . Line number %d .\n" , token, numline);
							}
						}
						if ( token[0] == '#')
						{
							(g->ic)++;		/* add 1 word */
						}
						else
						{
							if ( token[0] != 'r')
							{
								g->ic = g->ic + 2;	/* add 2 words */
							}
							else
							{
								if ( (strlen(token) == 1) || (isdigit(token[1]) == 0) )
								{
									g->errorflag = 1;
									
									printf ( "Register should have a number between 0 - 15 - %s . Line number %d .\n" , token, numline);
								}
							}
						}
					}
					
					
				}
				else		/* 1 argument command */
				{
					(g->ic)++;
					(g->ic)++;
					token = strtok(NULL , " ");	/* get argument */
					if ( strstr(token,"[") != NULL )		/* check for index methode */
					{
						int h = 0;
						while (token[h] != '[')
						{
							h++;
						}
						h++;
						if (token[h] != 'r')
						{
							g->errorflag = 1;
							printf ( "Register inside index should have the letter 'r' - %s . Line number %d .\n" , token, numline);
						}
						h++;
						h++;
						if (isdigit(token[h]) == 0)
						{
							g->errorflag = 1;
							printf ( "Register inside index should have a number between 10 - 15 - %s . Line number %d .\n" , token, numline);
						}
					}
					if ( token[0] == '#')
					{
						(g->ic)++;		/* add 1 word */
					}
					else
					{
						if ( token[0] != 'r')
						{
							g->ic = g->ic + 2;	/* add 2 words */
						}
					}
				}
				
			}
			
		}
		else			/* command was not found -> error */
		{
			g->errorflag = 1;
									
			printf ( "Invalid command - %s . Line number %d .\n" , token, numline);
		}
		

		
	/* ***************************************************************************************************************************** */
	commfoundflag = 0;	/* reset flag */
	labelflag = 0;		/* reset flag */
	doublenameflag = 0;		/* reset the flag */
	}	/* end of while (tav != EOF ) */



	if ( g->errorflag == 1)		/* error identification */
	{
		/* remove am file */
		remove(newname);
		free(newname);
		printf("errorflag : %d \n", g->errorflag);
		return;
	}
	else
	{
		rewind(fp);
		secwalk(fp, g, head, path);
		free(newname);
	}
	sectemp = head;
	
	while (sectemp != NULL)
	{
		temp = sectemp;
		sectemp = sectemp->next;
		free(temp);
		
	}
	return;
}
	

