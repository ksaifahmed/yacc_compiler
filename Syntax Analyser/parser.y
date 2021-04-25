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
extern int line_count;
extern int error_count;
FILE * lg;
FILE * err;

//SymbolTable *table;


void yyerror(char *s)
{
	printf("error: %s", s);
}

SymbolInfo* assignProduction(string name, string type, string nonterminal, FILE * log_p)
{
	fprintf(log_p, "Line %d: %s : %s\n\n%s", line_count, nonterminal.c_str() ,type.c_str(), name.c_str()); 
	return new SymbolInfo(name, type);
}
%}



%union{SymbolInfo* si; string* str;}
%token NEWLINE IF FOR DO INT FLOAT VOID SWITCH DEFAULT ELSE WHILE BREAK CHAR DOUBLE RETURN CASE CONTINUE PRINTLN
%token INCOP DECOP ASSIGNOP NOT LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD COMMA SEMICOLON
%token <si> LOGICOP RELOP ADDOP MULOP CONST_FLOAT CONST_INT ID

%type <si> start program unit func_declaration func_definition parameter_list compound_statement
%type <si> var_declaration type_specifier declaration_list statements statement expression_statement
%type <si> variable expression logic_expression rel_expression simple_expression term
%type <si> unary_expression factor argument_list arguments


// %left 
// %right

// %nonassoc 


%%

start : program
	{
		//write your code in this block in all the similar blocks below
	}
	;

program : program unit 
	| unit
	;
	
unit : var_declaration
     | func_declaration
     | func_definition
     ;
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
		| type_specifier ID LPAREN RPAREN SEMICOLON
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement
		| type_specifier ID LPAREN RPAREN compound_statement
 		;				


parameter_list  : parameter_list COMMA type_specifier ID
		| parameter_list COMMA type_specifier
 		| type_specifier ID
		| type_specifier
 		;

 		
compound_statement : LCURL statements RCURL
 		    | LCURL RCURL
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON
		 {
		     $$ = assignProduction($1->getName()+$2->getName()+";", "type_specifier declaration_list SEMICOLON", "var_declaration", lg);
		 }
 		 ;
 		 
type_specifier	: INT	{
				$$ = assignProduction("int", "INT", "type_specifier", lg);
			}
 		| FLOAT {
				$$ = assignProduction("float", "FLOAT", "type_specifier", lg);
			}
 		| VOID {
				$$ = assignProduction("void", "VOID", "type_specifier", lg);
			}
 		;
 		
declaration_list : declaration_list COMMA ID
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
 		  | ID
 		  | ID LTHIRD CONST_INT RTHIRD
 		  ;
 		  
statements : statement
	   | statements statement
	   ;
	   
statement : var_declaration
	  | expression_statement
	  | compound_statement
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement
	  | IF LPAREN expression RPAREN statement
	  | IF LPAREN expression RPAREN statement ELSE statement
	  | WHILE LPAREN expression RPAREN statement
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
	  | RETURN expression SEMICOLON
	  ;
	  
expression_statement 	: SEMICOLON			
			| expression SEMICOLON 
			;
	  
variable : ID 		
	 | ID LTHIRD expression RTHIRD 
	 ;
	 
 expression : logic_expression	
	   | variable ASSIGNOP logic_expression 	
	   ;
			
logic_expression : rel_expression 	
		 {
		 	$$ = assignProduction($1->getName(), "rel_expression", "logic_expression", lg);
		 }
		 | rel_expression LOGICOP rel_expression 	
		 {
		 	string type = "rel_expression LOGICOP rel_expression";
			$$ = assignProduction($1->getName()+$2->getName()+$3->getName(), type , "logic_expression", lg);
		 }
		 ;
			
rel_expression	: simple_expression 
		{
			$$ = assignProduction($1->getName(), "simple_expression", "rel_expression", lg);
		}
		| simple_expression RELOP simple_expression	
		{
			string type = "simple_expression RELOP simple_expression";
			$$ = assignProduction($1->getName()+$2->getName()+$3->getName(), type , "rel_expression", lg);
		}
		;
				
simple_expression : term 
		  {
		  	$$ = assignProduction($1->getName(), "term", "simple_expression", lg);
		  }
		  | simple_expression ADDOP term 
		  {
		  	string type = "simple_expression ADDOP term";
		  	$$ = assignProduction($1->getName()+$2->getName()+$3->getName(), type , "simple_expression", lg);
		  }
		  ;
					
term :	unary_expression
     {
     	$$ = assignProduction($1->getName(), "unary_expression", "term", lg);
     }
     |  term MULOP unary_expression
     {
     	$$ = assignProduction($1->getName()+$2->getName()+$3-getName(), "term MULOP unary_expression", "term", lg);
     }
     ;

unary_expression : ADDOP unary_expression  
		 {
		 	$$ = assignProduction($1->getName()+$2->getName(), "ADDOP unary_expression", "unary_expression", lg);	
		 }
		 | NOT unary_expression 
		 {
		 	$$ = assignProduction("!"+$2->getName(), "NOT unary_expression", "unary_expression", lg);	
		 }		 
		 | factor 
		 {
		 	$$ = assignProduction($1->getName(), "factor", "unary_expression", lg);	
		 }
		 ;
	
factor	: variable 
	{
		$$ = assignProduction($1->getName(), "variable", "factor", lg);		
	}
	| ID LPAREN argument_list RPAREN
	{
		$$ = assignProduction($1->getName()+"("+$3->getName()+")", "ID LPAREN argument_list RPAREN", "factor", lg);		
	}	
	| LPAREN expression RPAREN
	{
		$$ = assignProduction("("+$2->getName()+")", "LPAREN expression RPAREN", "factor", lg);		
	}	
	| CONST_INT 
	{
		$$ = assignProduction($1->getName(), "CONST_INT", "factor", lg);		
	}	
	| CONST_FLOAT
	{
		$$ = assignProduction($1->getName(), "CONST_FLOAT", "factor", lg);
	}	
	| variable INCOP 
	{
		$$ = assignProduction($1->getName()+"++", "variable INCOP", "factor", lg);
	}	
	| variable DECOP
	{
		$$ = assignProduction($1->getName()+"--", "variable DECOP", "factor", lg);
	}
	;
	
argument_list : arguments
		{
			$$ = assignProduction($1->getName(), "arguments", "arguments_list", lg);
		} 
		|
		;
	
arguments : arguments COMMA logic_expression
		{
			$$ = assignProduction($1->getName()+","+$3->getName(), "arguments COMMA logic_expression", "arguments", lg);
		}
	      | logic_expression 
	      {
	      		$$ = assignProduction($1->getName(), "arguments COMMA logic_expression", "arguments", lg);
	      }
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
	
	lg = fopen("log.txt", "w");
	err = fopen("error.txt", "w");
	
	yyin=fp;
	yyparse();
	

	fclose(yyin);
	fclose(lg);
	fclose(err);
	
	
	return 0;
}

