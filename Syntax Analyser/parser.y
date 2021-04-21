%{
#include<iostream>
using namespace std;
#include<cstdlib>
#include<string>
#include<cmath>
#include "symboltable.h"


//#define YYSTYPE SymbolInfo*
//#define YYSTYPE char *

using namespace std;

int yyparse(void);
int yylex(void);
extern FILE *yyin;

//SymbolTable *table;


void yyerror(char *s)
{
	printf("error: %s", s);
}


%}



%union{string* str; SymbolInfo* si;}
%token NEWLINE IF FOR DO INT FLOAT VOID SWITCH DEFAULT ELSE WHILE BREAK CHAR DOUBLE RETURN CASE CONTINUE
%token <str> INCOP LOGICOP RELOP ADDOP MULOP ASSIGNOP NOT LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD COMMA SEMICOLON
%token <SymbolInfo*> CONST_FLOAT CONST_INT ID


// %left 
// %right

// %nonassoc 


%%


newl		: INT NEWLINE { printf("hudai -_-\n"); }
		| NEWLINE
		;
%%
int main(int argc,char *argv[])
{
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	FILE * fp = fopen(argv[1],"r");
	if(fp==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	yyin=fp;
	yyparse();
	

	fclose(yyin);
	
	return 0;
}

