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

SymbolTable *symbolTable;
string data_type = "";
string value_type = "";
vector<SymbolInfo*> params;
vector<string> args;


void yyerror(char *s)
{
	fprintf(lg, "error: %s", s);
}

SymbolInfo* assignProduction(string name, string type, string nonterminal, FILE * log_p)
{
	fprintf(lg, "Line %d: %s : %s\n\n%s\n\n", line_count, nonterminal.c_str() ,type.c_str(), name.c_str()); 
	return new SymbolInfo(name, type);
}

void insertParams()
{
	for(int i=0; i<params.size(); i++)
	{
		SymbolInfo * s = params.at(i);
		symbolTable -> Insert(s->getName(), s->getType(), s->getDataType());	
	}
	//params.clear();
}

void printError(string msg)
{
	fprintf(lg, "Error at line %d: %s\n\n", line_count, msg.c_str());
	fprintf(err, "Error at line %d: %s\n\n", line_count, msg.c_str());
	error_count++;
}

bool FindParam(string name)
{
	for(int i=0; i<params.size(); i++)
	{
		SymbolInfo * s = params.at(i);
		if(!name.compare(s->getName())) return true;
	}
	return false;
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

%nonassoc FAKE_ELSE
%nonassoc ELSE


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
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN 
		{
			SymbolInfo * sp = symbolTable -> Lookup($2->getName());
			if(sp!=NULL)
			{
				string f_name = $2 -> getName();
				printError("Multiple declaration of "+f_name);
			}
		}
		SEMICOLON
		{
			string type = "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON";
			string name = $1->getName()+" "+$2->getName()+"("+$4->getName()+");";
			$$ = assignProduction(name, type, "func_declaration", lg);
			
			FunctionInfo f;
			for(int i=0; i<params.size(); i++)
			{
				SymbolInfo * s = params.at(i);
				f.insertParams(s->getName(), s->getType(), s->getDataType());
			}
			params.clear();
			symbolTable -> Insert($2->getName(), "ID", $1->getType(), "function", f);
			
			
		}
		| type_specifier ID LPAREN RPAREN 
		{
			SymbolInfo * sp = symbolTable -> Lookup($2->getName());
			if(sp!=NULL)
			{
				string f_name = $2 -> getName();
				printError("Multiple declaration of "+f_name);
			}	
		}
		SEMICOLON
		{
			string type = "type_specifier ID LPAREN RPAREN SEMICOLON";
			string name = $1->getName()+" "+$2->getName()+"();";
			$$ = assignProduction(name, type, "func_declaration", lg);
			
			FunctionInfo f;
			for(int i=0; i<params.size(); i++)
			{
				SymbolInfo * s = params.at(i);
				f.insertParams(s->getName(), s->getType(), s->getDataType());
			}
			params.clear();
			symbolTable -> Insert($2->getName(), "ID", $1->getType(), "function", f);
		}
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN 
		{
			//return type mismatch with declaration
			SymbolInfo * sp = symbolTable -> Lookup($2->getName());
			if(sp!=NULL)
			{
				string type = $1 -> getType();
				string f_name = $2 -> getName();
				if(type.compare(sp -> getDataType()) && !sp->getVarType().compare("function")) 
					printError("Return type mismatch with function declaration in function "+f_name);
				else if(!sp->getVarType().compare("function"))
				{
					if(sp->getFunctionInfo().param_num() != params.size())
						printError("Total number of arguments mismatch with declaration in function "+f_name);
					else
					{
						///params sequence
						for(int i=0; i<params.size(); i++)
						{
							SymbolInfo * s = params.at(i);
							if(!sp->getFunctionInfo().match(s->getName(), s->getDataType(), i))
							{
								printError("Parameter sequence/type mismatches with declaration");
								break;
							}
						}
					}
				}
				else printError("Multiple declaration of "+f_name);
			}
			else
			{
				FunctionInfo f;
				for(int i=0; i<params.size(); i++)
				{
					SymbolInfo * s = params.at(i);
					f.insertParams(s->getName(), s->getType(), s->getDataType());
				}
				symbolTable -> Insert($2->getName(), "ID", $1->getType(), "function", f);
			}
		}
		   compound_statement
		   {
		   	params.clear();
			string name = $1->getName()+" "+$2->getName()+"("+$4->getName()+")"+$7->getName();
			string type = "type_specifier ID LPAREN parameter_list RPAREN compound_statement";
			$$ = assignProduction(name, type, "func_definition", lg);
			
		   }
		  
		  
		  
		   
		| type_specifier ID LPAREN RPAREN 
		{ 
			SymbolInfo * sp = symbolTable -> Lookup($2->getName());
			if(sp!=NULL)
			{
				string type = $1 -> getType();
				string f_name = $2 -> getName();
				if(type.compare(sp -> getDataType()) && !sp->getVarType().compare("function")) 
					printError("Return type mismatch with function declaration in function "+f_name);
				else if(!sp->getVarType().compare("function"))
				{
					if(sp->getFunctionInfo().param_num() > 0)
						printError("no arguments, mismatch with declaration in function "+f_name);
				}
			}
			else
			{
				symbolTable -> Insert($2->getName(), "ID", $1->getType(), "function"); 
			}
			params.clear();
			
		}
		compound_statement
		  {
		  	params.clear();
			string name = $1->getName()+" "+$2->getName()+"()"+$6->getName();
			string type = "type_specifier ID LPAREN RPAREN compound_statement";
			$$ = assignProduction(name, type, "func_definition", lg);
		  } 
 		  ;				


parameter_list  : parameter_list COMMA type_specifier ID
		{
			
			if(FindParam($4->getName())) {
    				printError("Multiple declaration of a in parameter");
			}
			
			string type = "parameter_list COMMA type_specifier ID";
			$$ = assignProduction($1->getName()+","+$3->getName()+" "+$4->getName(), type, "parameter_list", lg);
			//symbolTable -> Insert($4->getName(), "ID", $3->getType());
			
			SymbolInfo * s = new SymbolInfo($4->getName(), "ID");
			s -> setDataType($3->getType());
			params.push_back(s);
			
		}
		| parameter_list COMMA type_specifier
		{
			string type = "parameter_list COMMA type_specifier";
			$$ = assignProduction($1->getName()+","+$3->getName(), type, "parameter_list", lg);
		}
 		| type_specifier ID
		{
			$$ = assignProduction($1->getName()+" "+$2->getName(), "type_specifier ID", "parameter_list", lg);
			//symbolTable -> Insert($2->getName(), "ID", $1->getType());
			
			SymbolInfo * s = new SymbolInfo($2->getName(), "ID");
			s -> setDataType($1->getType());
			params.push_back(s);
		}
		| type_specifier
		{
			$$ = assignProduction($1->getName(), "type_specifier", "parameter_list", lg);
		}
 		;

 		
compound_statement : LCURL {symbolTable -> EnterScope(); insertParams(); } statements RCURL
 		    {
 		    	$$ = assignProduction("{\n"+$3->getName()+"}\n", "LCURL statements RCURL", "compound_statement", lg);
 		    	symbolTable -> PrintInFile(lg);
 		    	symbolTable -> ExitScope();
 		    }
 		    | LCURL {symbolTable -> EnterScope(); insertParams(); } RCURL
 		    {
 		    	$$ = assignProduction("{\n}\n", "LCURL RCURL", "compound_statement", lg);
 		    	symbolTable -> PrintInFile(lg);
 		    	symbolTable -> ExitScope();
 		    }
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON
		 {
		     string type = "type_specifier declaration_list SEMICOLON";
		     string name = $1->getName()+" "+$2->getName()+";";
		     $$ = new SymbolInfo(name, type);
		     
		     fprintf(lg, "Line %d: var_declaration : %s\n\n", line_count, type.c_str());
	 	     if(!$1->getType().compare("VOID"))
	 	     {
	 		printError("Variable type cannot be void");
	 	     }	
	 	     fprintf(lg, "%s\n\n", name.c_str());
		 }
 		 ;
 		 
type_specifier	: INT	{
				$$ = assignProduction("int", "INT", "type_specifier", lg);
				data_type = "INT";
			}
 		| FLOAT {
				$$ = assignProduction("float", "FLOAT", "type_specifier", lg);
				data_type = "FLOAT";
			}
 		| VOID {
				$$ = assignProduction("void", "VOID", "type_specifier", lg);
				data_type = "VOID";
			}
 		;
 		
declaration_list : declaration_list COMMA ID
 		  {
 		  	string type = "declaration_list COMMA ID";
	 		if(symbolTable -> Insert($3->getName(), "ID", data_type, "single_var"))
		 		$$ = assignProduction($1->getName()+","+$3->getName(), type, "declaration_list", lg);
		 	else{
		 		printError("Multiple declaration of "+$3->getName());	
		 		$$ = assignProduction($1->getName()+","+$3->getName(), type, "declaration_list", lg);
		 	}	
 		  }
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
 		  {
 		  	string type = "declaration_list COMMA ID LTHIRD CONST_INT RTHIRD";
 		  	string name = $1->getName()+","+$3->getName()+"["+$5->getName()+"]";
 		  	if(symbolTable -> Insert($3->getName(), "ID", data_type, "array"))
	 			$$ = assignProduction(name, type, "declaration_list", lg);
	 		else{
	 			printError("Multiple declaration of "+$3->getName());
	 			$$ = assignProduction(name, type, "declaration_list", lg);
	 		}	
	 		
 		  }
 		  | ID
 		  {
 		  	if(symbolTable -> Insert($1->getName(), "ID", data_type, "single_var"))
	 			$$ = assignProduction($1->getName(), "ID", "declaration_list", lg);
	 		else{
	 			printError("Multiple declaration of "+$1->getName());
	 			$$ = assignProduction($1->getName(), "ID", "declaration_list", lg);
	 		}
 		  }
 		  | ID LTHIRD CONST_INT RTHIRD
 		  {
 		  	string type = "ID LTHIRD CONST_INT RTHIRD";
 		  	if(symbolTable -> Insert($1->getName(), "ID", data_type,"array"))
		 		$$ = assignProduction($1->getName()+"["+$3->getName()+"]", type, "declaration_list", lg);
		 	else{
		 		printError("Multiple declaration of "+$1->getName());
		 		$$ = assignProduction($1->getName()+"["+$3->getName()+"]", type, "declaration_list", lg);
		 	}	
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
	  	string name = "for ("+$3->getName()+$4->getName()+$5->getName()+") "+$7->getName();
	 	$$ = assignProduction(name, type, "statement", lg);
	  }
	  | IF LPAREN expression RPAREN statement %prec FAKE_ELSE
	  {
	  	string type = "IF LPAREN expression RPAREN statement";
	 	$$ = assignProduction("if ("+$3->getName()+") "+$5->getName(), type, "statement", lg);
	  }
	  | IF LPAREN expression RPAREN statement ELSE statement
	  {
	  	string type = "IF LPAREN expression RPAREN statement ELSE statement";
	 	$$ = assignProduction("if ("+$3->getName()+") "+$5->getName()+"else "+$7->getName(), type, "statement", lg);
	  }
	  | WHILE LPAREN expression RPAREN statement
	  {
	  	string type = "WHILE LPAREN expression RPAREN statement";
	 	$$ = assignProduction("while ("+$3->getName()+") "+$5->getName(), type, "statement", lg);
	  }	  
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
	  {
	  	string type = "PRINTLN LPAREN ID RPAREN SEMICOLON";
	  	string name = "printf("+$3->getName()+");";
	  	SymbolInfo * sp = symbolTable -> Lookup($3->getName());
	 	$$ = new SymbolInfo(name, type);
	 	
	 	fprintf(lg, "Line %d: variable : %s\n\n", line_count, type.c_str());
	 	if(sp==NULL)
	 	{
	 		printError("Undeclared variable "+$3->getName());
	 	}
	 	fprintf(lg, "%s\n\n", name.c_str());
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
 	 	SymbolInfo * sp = symbolTable -> Lookup($1->getName());
	 	
	 	string name = $1->getName();
	 	string type = "ID";
	 	$$ = new SymbolInfo(name, type);
	 	if(sp != NULL) $$ -> setDataType(sp -> getDataType());
	 	
	 	fprintf(lg, "Line %d: variable : %s\n\n", line_count, type.c_str());
	 	if(sp!=NULL){
	 		if(!sp->getVarType().compare("array")) printError("Type mismatch, " + $1->getName() + " is an array");
	 	}	
	 	else printError("Undeclared variable "+$1->getName());
	 	fprintf(lg, "%s\n\n", name.c_str());
	 } 		
	 | ID LTHIRD expression RTHIRD 
	 {
	 	string type = "ID LTHIRD expression RTHIRD";
	 	string name = $1->getName()+"["+$3->getName()+"]";
	 	
	 	$$ = new SymbolInfo(name, type);
	 	SymbolInfo * sp = symbolTable -> Lookup($1->getName());
	 	if(sp != NULL) $$ -> setDataType(sp -> getDataType());
	 	
	 	fprintf(lg, "Line %d: variable : %s\n\n", line_count, type.c_str());
	 	if(sp == NULL) printError("Undeclared variable "+$1->getName());
	 	if(sp!=NULL)
	 		if(sp->getVarType().compare("array")) printError($1->getName() + " not an array");
	 	if($3->getDataType().compare("INT")) printError("Expression inside third brackets not an integer");
	 	fprintf(lg, "%s\n\n", name.c_str());
	 }
	 ;
	 
expression : logic_expression	
 	   {
 	   	string name = $1->getName();
 	   	string type = "logic expression";
 	   	$$ = new SymbolInfo(name, type);
 	   	
 	   	fprintf(lg, "Line %d: expression : %s\n\n", line_count, type.c_str());
 	   	if(!$1->getDataType().compare("VOID")) printError("Expression contains type : void");
 	   	fprintf(lg, "%s\n\n", name.c_str());
 	   	
 	   	$$ -> setDataType($1->getDataType());
 	   	
 	   	
 	   }
	   | variable ASSIGNOP logic_expression 	
	   {
	   	string type = "variable ASSIGNOP logic_expression";
	   	string name = $1->getName()+"="+$3->getName();
	   	$$ = new SymbolInfo(name, type);
	   	
	   	//look-up var in table
	   	string var_name = "";
	   	if ($1->getName().find("[") != string::npos) {
        		 var_name = $1->getName().substr(0, $1->getName().find("["));
        	}else var_name = $1->getName();
        	SymbolInfo * sp = symbolTable -> Lookup(var_name);
	   	
	   	//print errors
	   	fprintf(lg, "Line %d: expression : %s\n\n", line_count, type.c_str());
	   	if(sp!=NULL)
	   	{
	   		string temp = "";
	   		if(!$3->getDataType().compare("VOID")) printError("Void function used in expression");
	   		else
	   		{
		   		if(!$3->getDataType().compare("NULL"));
		   		else if(!sp->getDataType().compare("FLOAT") && !$3->getDataType().compare("INT"));
		   		else if(sp->getDataType().compare($3->getDataType())) printError("Type Mismatch");
	   		}	   		
	   		$$ -> setDataType(sp->getDataType());
	   	}else $$ -> setDataType("NULL");
	   	fprintf(lg, "%s\n\n", name.c_str());
	   }
	   ;
			
logic_expression : rel_expression 	
		 {
		 	$$ = assignProduction($1->getName(), "rel_expression", "logic_expression", lg);
		 	$$ -> setDataType($1->getDataType());
		 }
		 | rel_expression LOGICOP rel_expression 	
		 {
		 	string type = "rel_expression LOGICOP rel_expression";
			$$ = assignProduction($1->getName()+$2->getName()+$3->getName(), type , "logic_expression", lg);
			
			if(!$1->getDataType().compare("VOID") || !$3->getDataType().compare("VOID")) 
				$$ -> setDataType("VOID");
			else $$ -> setDataType("INT");
		 }
		 ;
			
rel_expression	: simple_expression 
		{
			$$ = assignProduction($1->getName(), "simple_expression", "rel_expression", lg);
			$$ -> setDataType($1->getDataType());
		}
		| simple_expression RELOP simple_expression	
		{
			string type = "simple_expression RELOP simple_expression";
			$$ = assignProduction($1->getName()+$2->getName()+$3->getName(), type , "rel_expression", lg);
			
			if(!$1->getDataType().compare("VOID") || !$3->getDataType().compare("VOID")) 
				$$ -> setDataType("VOID");
			else $$ -> setDataType("INT");
		}
		;
				
simple_expression : term 
		  {
		  	$$ = assignProduction($1->getName(), "term", "simple_expression", lg);
		  	$$ -> setDataType($1->getDataType());
		  }
		  | simple_expression ADDOP term 
		  {
		  	string type = "simple_expression ADDOP term";
		  	$$ = assignProduction($1->getName()+$2->getName()+$3->getName(), type , "simple_expression", lg);
		  	
		  	if(!$1->getDataType().compare("VOID") || !$3->getDataType().compare("VOID")) 
				$$ -> setDataType("VOID");
		  	else if(!$1->getDataType().compare("FLOAT") || !$3->getDataType().compare("FLOAT")) 
				$$ -> setDataType("FLOAT");
			else $$ -> setDataType("INT");
		  }
		  ;
					
term :	unary_expression
     {
     	$$ = assignProduction($1->getName(), "unary_expression", "term", lg);
     	$$ -> setDataType($1->getDataType());
     }
     |  term MULOP unary_expression
     {
     	string name = $1->getName()+$2->getName()+$3->getName();
     	string type = "term MULOP unary_expression";
     	$$ = new SymbolInfo(name, type);
     	
     	fprintf(lg, "Line %d: term : %s\n\n", line_count, type.c_str());
     	
	if(!$2->getName().compare("%")){
		if($1->getDataType().compare("INT") || $3->getDataType().compare("INT")) printError("Non-Integer operand on modulus operator");
		else if(!$3->getName().compare("0"))
		{
			printError("Modulus by Zero");
		}
	}
	
	if(!$1->getDataType().compare("VOID") || !$3->getDataType().compare("VOID")) 
		$$ -> setDataType("VOID");
	else if(!$1->getDataType().compare("FLOAT") || !$3->getDataType().compare("FLOAT")) 
		$$ -> setDataType("FLOAT");
	else $$ -> setDataType("INT");		
	
	fprintf(lg, "%s\n\n", name.c_str());
     }
     ;

unary_expression : ADDOP unary_expression  
		 {
		 	$$ = assignProduction($1->getName()+$2->getName(), "ADDOP unary_expression", "unary_expression", lg);	
		 	if(!$2->getDataType().compare("FLOAT")) 
		 	{
		 		$$ -> setDataType("FLOAT");	
		 	}else if(!$2->getDataType().compare("VOID")) 
		 		$$ -> setDataType("VOID");	
		 	else $$ -> setDataType("INT");	
		 }
		 | NOT unary_expression 
		 {
		 	$$ = assignProduction("!"+$2->getName(), "NOT unary_expression", "unary_expression", lg);
		 	if(!$2->getDataType().compare("VOID"))
		 		 $$ -> setDataType("VOID");
		 	else $$ -> setDataType("INT");		
		 }		 
		 | factor 
		 {
		 	$$ = assignProduction($1->getName(), "factor", "unary_expression", lg);
			$$ -> setDataType($1->getDataType());				
		 }
		 ;
	
factor	: variable 
	{
		$$ = assignProduction($1->getName(), "variable", "factor", lg);
		$$ -> setDataType($1->getDataType());		
	}
	| ID LPAREN argument_list RPAREN
	{
		string name = $1->getName()+"("+$3->getName()+")";
		string type = "ID LPAREN argument_list RPAREN";
		$$ = new SymbolInfo(name, type);
		
		fprintf(lg, "Line %d: factor : %s\n\n", line_count, type.c_str());
		
		SymbolInfo * sp = symbolTable -> Lookup($1->getName());
		if(sp != NULL) {
			$$ -> setDataType(sp->getDataType());
			if(!sp->getVarType().compare("function"))
			{
				if(sp->getFunctionInfo().param_num() != args.size())
					printError("Total number of arguments mismatch in function "+$1->getName());
				else
				{
					///args sequence
					for(int i=0; i<args.size(); i++)
					{
						string arg = args.at(i);
						if(sp->getFunctionInfo().match("FLOAT", i) && !arg.compare("INT")) continue;
						if(!sp->getFunctionInfo().match(arg, i))
						{
							i++;
							printError(to_string(i)+"th argument mismatch in function "+$1->getName());
							break;
						}
					}
				}
			}
			else printError($1->getName()+" is not a function");	
			
		}
		else {
			printError("Undeclared function "+$1->getName());	
			$$ -> setDataType("NULL");
		}
		args.clear();
		fprintf(lg, "%s\n\n", name.c_str());
					
	}	
	| LPAREN expression RPAREN
	{
		$$ = assignProduction("("+$2->getName()+")", "LPAREN expression RPAREN", "factor", lg);
		$$ -> setDataType($2->getDataType());			
	}	
	| CONST_INT 
	{
		$$ = assignProduction($1->getName(), "CONST_INT", "factor", lg);	
		value_type = "INT";	
		$$ -> setDataType("INT");	
	}	
	| CONST_FLOAT
	{
		$$ = assignProduction($1->getName(), "CONST_FLOAT", "factor", lg);
		value_type = "FLOAT";
		$$ -> setDataType("FLOAT");	
	}	
	| variable INCOP 
	{
		$$ = assignProduction($1->getName()+"++", "variable INCOP", "factor", lg);
		$$ -> setDataType($1->getDataType());	
	}	
	| variable DECOP
	{
		$$ = assignProduction($1->getName()+"--", "variable DECOP", "factor", lg);
		$$ -> setDataType($1->getDataType());
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
			args.push_back($3->getDataType());
			
		}
	      | logic_expression 
	      {
	      		$$ = assignProduction($1->getName(), "logic_expression", "arguments", lg);
			args.push_back($1->getDataType());
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
	symbolTable = new SymbolTable(30);
	
	yyin=fp;
	yyparse();
	
	
	symbolTable -> PrintInFile(lg);
	fprintf(lg, "\nTotal lines: %d\nTotal errors: %d\n\n", line_count, error_count);
	
	
	fclose(yyin);
	fclose(lg);
	fclose(err);
	
	
	return 0;
}

