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
	fprintf(lg, "error: %s", s);
}

SymbolInfo* assignProduction(string name, string type, string nonterminal, FILE * log_p)
{
	fprintf(log_p, "Line %d: %s : %s\n\n%s\n\n", line_count, nonterminal.c_str() ,type.c_str(), name.c_str()); 
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
		//$$ = assignProduction($1->getName(), "program", "start", lg);
		fprintf(lg, "Line %d: start : program\n\n", line_count); 
	}
	;

program : program unit 
	{
		$$ = assignProduction($1->getName()+$2->getName()+"\n", "program unit", "program", lg);
	}
	| unit
	{
		$$ = assignProduction($1->getName()+"\n", "unit", "program", lg);
	}
	;
	
unit : var_declaration
     {
     	$$ = assignProduction($1->getName(), "var_declaration", "unit", lg);
     }
     | func_declaration
     {
     	$$ = assignProduction($1->getName(), "func_declaration", "unit", lg);
     }
     | func_definition
     {
     	$$ = assignProduction($1->getName(), "func_definition", "unit", lg);
     }
     ;
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
		{
			string type = "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON";
			string name = $1->getName()+" "+$2->getName()+"("+$4->getName()+");";
			$$ = assignProduction(name, type, "func_declaration", lg);
		}
		| type_specifier ID LPAREN RPAREN SEMICOLON
		{
			string type = "type_specifier ID LPAREN RPAREN SEMICOLON";
			string name = $1->getName()+" "+$2->getName()+"();";
			$$ = assignProduction(name, type, "func_declaration", lg);
		}
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement
		{
			string name = $1->getName()+" "+$2->getName()+"("+$4->getName()+")"+$6->getName();
			string type = "type_specifier ID LPAREN parameter_list RPAREN compound_statement";
			$$ = assignProduction(name, type, "func_definition", lg);
		}
		| type_specifier ID LPAREN RPAREN compound_statement
		{
			string name = $1->getName()+" "+$2->getName()+"()"+$5->getName();
			string type = "type_specifier ID LPAREN RPAREN compound_statement";
			$$ = assignProduction(name, type, "func_definition", lg);
		}
 		;				


parameter_list  : parameter_list COMMA type_specifier ID
		{
			string type = "parameter_list COMMA type_specifier ID";
			$$ = assignProduction($1->getName()+","+$3->getName()+" "+$4->getName(), type, "parameter_list", lg);
		}
		| parameter_list COMMA type_specifier
		{
			string type = "parameter_list COMMA type_specifier";
			$$ = assignProduction($1->getName()+","+$3->getName(), type, "parameter_list", lg);
		}
 		| type_specifier ID
		{
			$$ = assignProduction($1->getName()+" "+$2->getName(), "type_specifier ID", "parameter_list", lg);
		}
		| type_specifier
		{
			$$ = assignProduction($1->getName(), "type_specifier", "parameter_list", lg);
		}
 		;

 		
compound_statement : LCURL statements RCURL
 		    {
 		    	$$ = assignProduction("{\n"+$2->getName()+"}\n", "LCURL statements RCURL", "compound_statement", lg);
 		    }
 		    | LCURL RCURL
 		    {
 		    	$$ = assignProduction("{\n}\n", "LCURL RCURL", "compound_statement", lg);
 		    }
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON
		 {
		     string type = "type_specifier declaration_list SEMICOLON";
		     $$ = assignProduction($1->getName()+" "+$2->getName()+";", type, "var_declaration", lg);
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
 		  {
 		  	string type = "declaration_list COMMA ID";
	 		$$ = assignProduction($1->getName()+","+$3->getName(), type, "declaration_list", lg);
 		  }
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
 		  {
 		  	string type = "declaration_list COMMA ID LTHIRD CONST_INT RTHIRD";
 		  	string name = $1->getName()+","+$3->getName()+"["+$5->getName()+"]";
	 		$$ = assignProduction(name, type, "declaration_list", lg);
 		  }
 		  | ID
 		  {
	 		$$ = assignProduction($1->getName(), "ID", "declaration_list", lg);
 		  }
 		  | ID LTHIRD CONST_INT RTHIRD
 		  {
 		  	string type = "ID LTHIRD CONST_INT RTHIRD";
	 		$$ = assignProduction($1->getName()+"["+$3->getName()+"]", type, "declaration_list", lg);
 		  }
 		  ;
 		  
statements : statement
	   {
	   	$$ = assignProduction($1->getName()+"\n", "statement", "statements", lg);
	   }
	   | statements statement
	   {
	   	$$ = assignProduction($1->getName()+$2->getName()+"\n", "statements statement", "statements", lg);
	   }
	   ;
	   
statement : var_declaration
	  {
	 	$$ = assignProduction($1->getName(), "var_declaration", "statement", lg);
	  }
	  | expression_statement
	  {
	 	$$ = assignProduction($1->getName(), "expression_statement", "statement", lg);
	  }
	  | compound_statement
	  {
	 	$$ = assignProduction($1->getName(), "compound_statement", "statement", lg);
	  }
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement
	  {
	  	string type = "FOR LPAREN expression_statement expression_statement expression RPAREN statement";
	  	string name = "for("+$3->getName()+$4->getName()+$5->getName()+")"+$7->getName();
	 	$$ = assignProduction(name, type, "statement", lg);
	  }
	  | IF LPAREN expression RPAREN statement
	  {
	  	string type = "IF LPAREN expression RPAREN statement";
	 	$$ = assignProduction("if("+$3->getName()+")"+$5->getName(), type, "statement", lg);
	  }
	  | IF LPAREN expression RPAREN statement ELSE statement
	  {
	  	string type = "IF LPAREN expression RPAREN statement ELSE statement";
	 	$$ = assignProduction("if("+$3->getName()+")"+$5->getName()+"else "+$7->getName(), type, "statement", lg);
	  }
	  | WHILE LPAREN expression RPAREN statement
	  {
	  	string type = "WHILE LPAREN expression RPAREN statement";
	 	$$ = assignProduction("while("+$3->getName()+")"+$5->getName(), type, "statement", lg);
	  }	  
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
	  {
	  	string type = "PRINTLN LPAREN ID RPAREN SEMICOLON";
	 	$$ = assignProduction("printf("+$3->getName()+");", type, "statement", lg);
	  }
	  | RETURN expression SEMICOLON
	  {
	  	string type = "RETURN expression SEMICOLON";
	 	$$ = assignProduction("return "+$2->getName()+";", type, "statement", lg);
	  }
	  ;
	  
expression_statement 	: SEMICOLON
			{
				$$ = assignProduction(";", "SEMICOLON", "expression_statement", lg);
			}			
			| expression SEMICOLON 
			{
				$$ = assignProduction($1->getName()+";", "expression SEMICOLON", "expression_statement", lg);
			}
			;
	  
variable : ID
	 {
	 	$$ = assignProduction($1->getName(), "ID", "variable", lg);
	 } 		
	 | ID LTHIRD expression RTHIRD 
	 {
	 	string type = "ID LTHIRD expression RTHIRD";
	 	$$ = assignProduction($1->getName()+"["+$3->getName()+"]", type, "variable", lg);
	 }
	 ;
	 
expression : logic_expression	
 	   {
 	   	$$ = assignProduction($1->getName(), "logic_expression", "expression", lg);
 	   }
	   | variable ASSIGNOP logic_expression 	
	   {
	   	string type = "variable ASSIGNOP logic_expression";
	   	$$ = assignProduction($1->getName()+"="+$3->getName(), type, "expression", lg);
	   }
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
     	$$ = assignProduction($1->getName()+$2->getName()+$3->getName(), "term MULOP unary_expression", "term", lg);
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
			$$ = assignProduction($1->getName(), "arguments", "argument_list", lg);
		} 
		|
		{
			$$ = assignProduction("", "", "argument_list", lg);
		}
		;
	
arguments : arguments COMMA logic_expression
		{
			$$ = assignProduction($1->getName()+","+$3->getName(), "arguments COMMA logic_expression", "arguments", lg);
		}
	      | logic_expression 
	      {
	      		$$ = assignProduction($1->getName(), "logic_expression", "arguments", lg);
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
	
	
	fprintf(lg, "\nTotal lines: %d\nTotal errors: %d\n\n", line_count, error_count);
	
	
	fclose(yyin);
	fclose(lg);
	fclose(err);
	
	
	return 0;
}

