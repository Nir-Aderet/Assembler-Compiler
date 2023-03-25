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

struct CTNode {				/* command table node */
	char[4] name;
	char[5] sname;
	char[20] opcode;
	char[9] funct;

//	char[5] destaddress;	/* destenation-address */
//	char[5] origaddress;	/* origin-address */
//	char[5] amdest;	/* addressing-methode-destenation */
//	char[5] amorig; /* addressing-methode-origin */

	struct CTNode* next = NULL;
};

struct MNode {				/* macro table node */
	char[32] name;
	char * body;
	struct *MNode next = NULL;
};

struct LTNode {				/* label table node */
	char[32] name;
	//char line[MAX_LINE_LENGTH] = {0};	/* for use in data/string command */
	int val;
	int base;
	int offset;
	int ext = 0;
	int ent = 0;
	int code = 0;
	int data = 0;
	struct *LTNode next = NULL;
};


struct global {
	int errorflag = 0;
	int ic = 100;
	int dc = 0;
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




