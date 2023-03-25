#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "sidefunctions.h"

void fourbitsbin(int number, char *a)
{
	int c;
	int i = 4;

	while(number  != 0)			/* transform a decimal number to binary number */
    	{
    		c = number % 2;
    		if (c == 1)
    		{
    			a[i] = '1';
    		}
    		else
    		{
    			a[i] = '0';
    		}
       		number = number / 2;
       		i--;
   	 }
  }




/* function gets 5 pointers to strings and the start of the label list,
first is an input string
second is output string, should be reset before
third is for second output, should be reset before
forth is register num and addresing methode length of 6 bits, should be reset before
fifth is label word( or ARE section) of first 4 bits in base and offset, should be reset before
first string is the arrtibute, second and third in some cases are the arrtibute in binary */
int arrtobin(char *ar, char *fw, char *sw, char *rw, char *lw, struct LTNode *head, int linenum)
{
	/*		0100 funct rw(first) rw(second)
	optional:	lw(first) fw(first)
	optional:	lw(first) sw(first)
	optional:	lw(second) fw(second)
	optional:	lw(second) sw(second)
	*/
	int l;
	for (l=0; l <= 15; ++l)		/* reset the word */
	{
		fw[l] = '0';
		sw[l] = '0';
	}
	char* token;				/*^^ ?????? pointer to pointer head ????? ^^*/
	char temp[32];		
	int indexflag = 0;
	int ind;
	struct LTNode *search = head;
	if ((ar[0]) == '#')	/* use only fw and rw - immediate  ar[0] == '#' ;;  strstr(ar,"#") != NULL ;; strncmp(ar[0], "#", 1) == 0 */
	{
		int number;
		token = strtok(ar, "#");		/* get the number */
		number = atoi(token);			/* transform it to int type */
		/*dectobin(n, fw);			 put it's binary form into fw */
		int b;
		int i;
		i = 15;
					
		while(number  != 0)			/* transform a decimal number to binary number */
    		{
    			b = number % 2;
    			if (b == 1)
    			{
    				fw[i] = '1';
    			}
    			else
    			{
    				fw[i] = '0';
    			}
       			number = number / 2;
       			i--;
   		}
		for (ind = 5; ind >= 0; ind--)
		{
				rw[ind] = '0';
		}
		/*rw = "000000";*/
		for (ind = 3; ind >= 0; ind--)
		{
			if (ind == 1)
			{
				lw[ind] = '1';
			}
			else
			{
				lw[ind] = '0';
			}
		}
   	 	/*lw = "0100";*/
		return 1;
	}
	
	if ( strstr(ar,"[r1") != NULL )		/* check for index methode */
	{
		indexflag = 1;
		token = strtok(ar, "[");	/* get label name */
		strcpy(temp,token);		/* copy label name */
		
	}
	
	
	if ( (ar[0] == 'r') || (indexflag) )	/* use only rw - register direct */
	{
		int number;
		if (indexflag)
		{
			token = strtok(NULL, "[");
			token = strtok(token, "]");	/* get register */
			token = strtok(token, "r");	/* get refister number */
		}
		else
		{
			token = strtok(ar, "r");		/* get the register number */
		}
		number = atoi(token);
		
		int c;
		int i = 3;
		
		for (ind = 3; ind >= 0; ind--)
		{
			rw[ind] = '0';
		}
		while(number  != 0)			/* transform a decimal number to binary number */
    		{
    			c = number % 2;
    			if (c == 1)
    			{
    				rw[i] = '1';
    			}
    			else
    			{
    				rw[i] = '0';
    			}
       			number = number / 2;
       			i--;
   	 	}
   	 	if (indexflag)		/* set addresing methode */
		{
			strcat(rw, "10");	/* index */
		}
		else
		{
   	 		strcat(rw, "11");	/* register direct */
   	 		for (ind = 3; ind >= 0; ind--)
			{
				if (ind == 1)
				{
					lw[ind] = '1';
				}
				else
				{
					lw[ind] = '0';
				}
			}
   	 		/*lw = "0100";*/
   	 		return 0;
		}
	}
	if (!(indexflag))		/* direct */
	{
		strcpy(temp,ar);		/* copy label name */
		for (ind = 5; ind >= 0; ind--)
		{
			if (ind == 5)
			{
				rw[ind] = '1';
			}
			else
			{
				rw[ind] = '0';
			}
		}
		/*rw = "000001";*/
	}
	while (search != NULL)		/* search for label name in list */
	{
		if ( strcmp(temp, search->name ) == 0)
		{
			
			if (search->ext == 1)		/* external label */
			{
				search->base = linenum;
				search->offset = linenum+1;
				for (ind = 15; ind >= 0; ind--)
				{
					fw[ind] = '0';
					sw[ind] = '0';
				}
				/*fw = "0000000000000000";
				sw = "0000000000000000";*/
				for (ind = 3; ind >= 0; ind--)
				{
					if (ind == 3)
					{
						lw[ind] = '1';
					}
					else
					{
						lw[ind] = '0';
					}
				}
   	 			/*lw = "0001";*/
				return 2;
			}
							/* relocatable label */
			int number;
			int b;
			int i;
			for (ind = 0; ind < 2; ind++)
			{
				i = 15;
				if (ind == 0)
				{
					number = search->base;
					
					while(number  != 0)			/* transform a decimal number to binary number */
    					{
    						b = number % 2;
    						if (b == 1)
    						{
    							fw[i] = '1';
    						}
    						else
    						{
    							fw[i] = '0';
    						}
       						number = number / 2;
       						i--;
   					}
				}
				else
				{
					number = search->offset;
					while(number  != 0)			/* transform a decimal number to binary number */
    					{
    						b = number % 2;
    						if (b == 1)
    						{
    							sw[i] = '1';
    						}
    						else
    						{
    							sw[i] = '0';
    						}
       						number = number / 2;
       						i--;
   					}
				}
				
			}
			for (ind = 3; ind >= 0; ind--)
			{
				if (ind == 2)
				{
					lw[ind] = '1';
				}
				else
				{
					lw[ind] = '0';
				}
			}
   	 		/*lw = "0010";*/
			return 2;
		}
		search= search->next;
	}
	return 0;
}

void resetposi(char *word, int n)
{
	int l;
	for (l=0; l < n; ++l)		/* reset the word */
	{
		word[l] = '0';
	}
	return;
}




/* function gets a pointer to a string and transforms all the charecters to '\0' */
/* n is the number of charecters in the string */
void resetword(char *word, int n)
{
	int l;
	for (l=0; l < n; ++l)		/* reset the word */
	{
		word[l] = '\0';
	}
	return;
}


/* input: num - number to be translated to string between 100 - 8191
	  *b - pointer to the string in wich we put the transelated integer, should be length of 5 with \0 */
void inttostring(int num, char *b)
{
    	int c;
    	int i = 3;
    	while (num != 0)
    	{
      	  	c = num % 10;
        	if (c == 0)
        	{
            		b[i] = '0';
        	}
        	if (c == 1)
        	{
            		b[i] = '1';
        	}
        	if (c == 2)
        	{
            		b[i] = '2';
        	}
        	if (c == 3)
        	{
            		b[i] = '3';
        	}
        	if (c == 4)
        	{
            		b[i] = '4';
        	}
        	if (c == 5)
        	{
            		b[i] = '5';
        	}
        	if (c == 6)
        	{
            		b[i] = '6';
        	}
        	if (c == 7)
        	{
            		b[i] = '7';
        	}
        	if (c == 8)
        	{
            		b[i] = '8';
        	}
        	if (c == 9)
        	{
            		b[i] = '9';
        	}
        	i--;
        	num = num / 10;
    	}
    	return;
}


/* function gets a number  to transform to binary in 16 bits and put it in the string that the pointer points at */
/* the string should be initialized to 16 zeros */
void dectobin(int number, char *a)
{
	int l;
	for (l=0; l <= 15; ++l)		/* reset the word */
	{
		a[l] = '0';
	}
	int c;
	int i = 15;
	int negativeflag = 0;
	
	if (number < 0)
	{
		number = number * (-1);
		negativeflag = 1;
	}
	while(number  != 0)			/* transform a decimal number to binary number */
    	{
    		c = number % 2;
    		if (c == 1)
    		{
    			a[i] = '1';
    		}
    		else
    		{
    			a[i] = '0';
    		}
       		number = number / 2;
       		i--;
   	 }
	if (negativeflag)
	{
		i = 0;
		for (; i<16; i++)		/* flip all bits */
		{
			if (a[i] == '0')
			{
				a[i] = '1';
			}
			else
			{
				a[i] = '0';
			}
		}
		i = 15;
		while (a[i] == '1')		/* add 1 */
		{
			a[i] = '0';
			i--;
		}
		a[i] = '1';
		return;
	}
	else
	{
		return;
	}
	
	
}


/* --- get line from file --- */
int getline(FILE *fp, char *line)
{
	char tav;				/* next char in line*/
	
	int i = 0;			/* index for parameter line*/
	
	resetword(line, MAX_LINE_LENGTH+1);
	tav = '\0';
	while (tav != '\n')		/*get the next line in file*/
	{

	        tav = fgetc(fp);
	        if (tav == EOF)			/* EOF */
	        {
	        	/*fclose(fp);		 close the file */
	        	return 0;
	        }
	       	if ((i == 0) && ((tav ==' ') || (tav == '\t') || (tav == '\n') ) )
	       	{
	       		continue;		/* skip all whitespace in the start of the line */
	       	}
	       	if ( (i > 0) && (line[i-1] == ',') && ((tav ==' ') || (tav == '\t') ) )
	       	{
	       		continue;		/* skip all whitespace after commas */
	       	}
        	if ( (i > 1) && (line[i-1] == ',') && (line[i-2] == ' ') )
	        {
	       		line[i-2] = ',';	/* earse a space that comes before a comma */
	       		line[i-1] = tav;
	       		continue;
	       	}
		if (tav == ';')
		{
			while ((tav != '\n') && (tav != EOF ))		/*get the next line in file*/
			{
				tav = fgetc(fp);
			}		/* ----- if the line is a note then ignore it ----- */
			if (tav == EOF)			/* EOF */
	        	{
	        	/*fclose(fp);		 close the file */
	       	 		return 0;
	        	}
	        	else
	        	{
	        		line[i] = '\0';
	        		return 1;
	        	}
		}
		line[i] = tav;
		i++;
	  }		/* end of while (tav != '\n') */
	  	line[i] = '\0';
	  	return 1;
}

/* bin = pointer to string that holds the 20 bits of the word
   sb = pointer to output special base, length of 15 with \0 */

void bintospecialbase(char *bin, char *sb)
{
	int l;
	for (l=0; l < 15; ++l)		/* reset the word */
	{
		sb[l] = '\0';
	}
	char aa[5];
	int i = 0;
	int j = 0;
	sb[j] = 'A';
	j++;
	while (i < 19)
	{
		for (l=0; l < 5; ++l)		/* reset the word */
		{
			aa[l] = '\0';
		}
		if ( j == 2)
		{
			sb[j] = '-';
			j++;
			sb[j] = 'B';
			j++;
		}
		if ( j == 5)
		{
			sb[j] = '-';
			j++;
			sb[j] = 'C';
			j++;
		}
		if ( j == 8)
		{
			sb[j] = '-';
			j++;
			sb[j] = 'D';
			j++;
		}
		if ( j == 11)
		{
			sb[j] = '-';
			j++;
			sb[j] = 'E';
			j++;
		}
		for (l = 0; l < 5; l++)
		{
			aa[l] = '\0';
		}
		aa[0] = bin[i];
		i++;
		aa[1] = bin[i];
		i++;
		aa[2] = bin[i];
		i++;
		aa[3] = bin[i];
		i++;
		if (strcmp(aa, "0000") == 0)
		{
			sb[j] = '0';
			j++;
			continue;
		}
		if (strcmp(aa, "0001") == 0)
		{
			sb[j] = '1';
			j++;
			continue;
		}
		if (strcmp(aa, "0010") == 0)
		{
			sb[j] = '2';
			j++;
			continue;
		}
		if (strcmp(aa, "0011") == 0)
		{
			sb[j] = '3';
			j++;
			continue;
		}
		if (strcmp(aa, "0100") == 0)
		{
			sb[j] = '4';
			j++;
			continue;
		}
		if (strcmp(aa, "0101") == 0)
		{
			sb[j] = '5';
			j++;
			continue;
		}
		if (strcmp(aa, "0110") == 0)
		{
			sb[j] = '6';
			j++;
			continue;
		}
		if (strcmp(aa, "0111") == 0)
		{
			sb[j] = '7';
			j++;
			continue;
		}
		if (strcmp(aa, "1000") == 0)
		{
			sb[j] = '8';
			j++;
			continue;
		}
		if (strcmp(aa, "1001") == 0)
		{
			sb[j] = '9';
			j++;
			continue;
		}
		if (strcmp(aa, "1010") == 0)
		{
			sb[j] = 'a';
			j++;
			continue;
		}
		if (strcmp(aa, "1011") == 0)
		{
			sb[j] = 'b';
			j++;
			continue;
		}
		if (strcmp(aa, "1100") == 0)
		{
			sb[j] = 'c';
			j++;
			continue;
		}
		if (strcmp(aa, "1101") == 0)
		{
			sb[j] = 'd';
			j++;
			continue;
		}
		if (strcmp(aa, "1110") == 0)
		{
			sb[j] = 'e';
			j++;
			continue;
		}
		if (strcmp(aa, "1111") == 0)
		{
			sb[j] = 'f';
			j++;
			continue;
		}
	}
	return;
}


void initglobal(struct global *g)
{
	g->errorflag = 0;
	g->ic = 99;
	g->dc = 0;
}
