/*

global parameters / structures : 
	- commandes table - list		[CTNode]
	- addressing mathodes table - list	
	- error flag - int			[errorflag]
	- label table - list			[LTNode]
	- instructions and command list - array	[InsComList]

*/

#define MAX_LINE_LENGTH 82
#define OUTPUT_FILE_LINE_LENGTH 106
/*
struct CTNode {				 command table node 
	char name[4];
	char sname[5];
	char opcode[20];
	char funct[9];

	char[5] destaddress;	 destenation-address 
	char[5] origaddress;	 origin-address 
	char[5] amdest;	 addressing-methode-destenation 
	char[5] amorig;  addressing-methode-origin 

	struct CTNode *next; 
	  
}; */

struct MNode {				/* macro table node */
	char name[32];
	char * body;
	struct MNode *next;
	/*  */
};

struct LTNode {				/* label table node */
	char name[32];
	/*char line[MAX_LINE_LENGTH] = {0};	 for use in data/string command */
	int val;
	int base;
	int offset;
	int ext;
	int ent;
	int code;
	int data;
	struct LTNode *next;
	/*  */
};


struct global {
	int errorflag;
	int ic;
	int dc;
};


/*
struct Entextnode {
	char[32] name;
	int ext = 0;
	int ent = 0;
	struct *Entextnode next = NULL;
};
*/
/*
struct Word {
	char[20] word;
};
*/




