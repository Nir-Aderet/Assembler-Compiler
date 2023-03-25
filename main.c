#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "firstwalk.h"
#include "structs.h"
#include "sidefunctions.h"
#include "preassmbler.h"


void initglobal(struct global *g);

int main (int argc, char *argv[])
{
	int i;
	if ( argc < 1)
	{
		printf("No files inserted.\n");
		return 0;
	}
	for ( i=1; i<argc; i++)
	{
		/* struct global glob; */
		struct global *g;
		g = malloc(sizeof( struct global));
		initglobal(g);
		preassmbler(argv[i], g);
		firstWalk(argv[i], g);
		free(g);
		
	}
	return 0;
}






