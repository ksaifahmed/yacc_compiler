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
FILE * ac;
FILE * oac;

bool isMain;
SymbolTable *symbolTable;
string data_type = "";
string value_type = "";
vector<SymbolInfo*> params;
vector<string> args;

//asm .data section
vector<SymbolInfo*> asm_vars;

int labelCount=0;
int tempCount=0;


char *newLabel()
{
	char *lb= new char[4];
	strcpy(lb,"L");
	char b[3];
	sprintf(b,"%d", labelCount);
	labelCount++;
	strcat(lb,b);
	return lb;
}

char *newTemp()
{
	char *t= new char[4];
	strcpy(t,"t");
	char b[3];
	sprintf(b,"%d", tempCount);
	tempCount++;
	strcat(t,b);
	
	//push in vector for .data section
	SymbolInfo * s_temp = new SymbolInfo(string(t), "notarray");
	asm_vars.push_back(s_temp);	
	
	return t;
}


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

string getDataAsm()
{
	string str = "\n.data\n";
	for(int i = 0; i < asm_vars.size(); i++)
	{
		SymbolInfo * temp = asm_vars.at(i);
		if(!temp->getType().compare("notarray"))
			str += "\t"+temp->getName()+" dw ?\n";
		else
			str += "\t"+temp->getName()+" dw " + temp->getType() + " dup(?)\n";
	}
	return str;
}

string PrintFunction(){
	return "\n\nprint PROC\n\tpush ax\n\tpush bx \n\tpush cx\n\tpush dx\n\tmov ax, print_var\n\tmov bx, 10\n\tmov cx, 0\nprintLabel1:\n\tmov dx, 0\n\tdiv bx\n\tpush dx\n\tinc cx\n\tcmp ax, 0\n\tjne printLabel1\nprintLabel2:\n\tmov ah, 2\n\tpop dx\n\tadd dl, '0'\n\tint 21h\n\tdec cx\n\tcmp cx, 0\n\tjne printLabel2\n\tmov dl, 0Ah\n\tint 21h\n\tmov dl, 0Dh\n\tint 21h\n\tpop dx\n\tpop cx\n\tpop bx\n\tpop ax\n\tret\nprint endp\n\n";
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
		
		//headers
		fprintf(ac, ".model small\n.stack 100h\n"); 
		
		//data
		fprintf(ac, "%s\n", getDataAsm().c_str());
		
		//println
		fprintf(ac, ".code%s", PrintFunction().c_str()); 
		
		//codes
		fprintf(ac, "%s", $1->code.c_str()); 
	}
	;

program : program unit 
	{
		$$ = assignProduction($1->getName()+$2->getName()+"\n", "program unit", "program", lg);
	     	$$->code += $1->code + $2->code;
	}
	| unit
	{
		$$ = assignProduction($1->getName()+"\n", "unit", "program", lg);
	     	$$->code += $1->code;		
	}
	;
	
unit : var_declaration
     {
     	$$ = assignProduction($1->getName(), "var_declaration", "unit", lg);
     	$$->code = "";     	     	
     }
     | func_declaration
     {
     	$$ = assignProduction($1->getName(), "func_declaration", "unit", lg);
     	$$->code += $1->code;     	
     }
     | func_definition
     {
     	$$ = assignProduction($1->getName(), "func_definition", "unit", lg);
     	$$->code += $1->code;
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
			
			if(!$2->getName().compare("main"))
			{
				$$->code += "main proc\nmov ax,@data\nmov ds,ax\n";
				$$->code += $6 -> code;
				$$->code += "exit:\nmov ah,4ch\nint 21h\nmain endp\nend main";
			}			
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
 		    	$$->code += $3->code;
 		    }
 		    | LCURL {symbolTable -> EnterScope(); insertParams(); } RCURL
 		    {
 		    	$$ = assignProduction("{\n}\n", "LCURL RCURL", "compound_statement", lg);
 		    	symbolTable -> PrintInFile(lg);
 		    	symbolTable -> ExitScope();
 		    	$$->code = "";
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
		 		 		
	 		SymbolInfo * temp = new SymbolInfo($3->getName()+symbolTable->getID(), "notarray");
	 		asm_vars.push_back(temp);
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
	 		
	 		//array	 		
	 		SymbolInfo * temp = new SymbolInfo($3->getName()+symbolTable->getID(), $5->getName());
	 		asm_vars.push_back(temp);		
	 		
 		  }
 		  | ID
 		  {
 		  	if(symbolTable -> Insert($1->getName(), "ID", data_type, "single_var"))
	 			$$ = assignProduction($1->getName(), "ID", "declaration_list", lg);
	 		else{
	 			printError("Multiple declaration of "+$1->getName());
	 			$$ = assignProduction($1->getName(), "ID", "declaration_list", lg);
	 		}
	 		
	 		SymbolInfo * temp = new SymbolInfo($1->getName()+symbolTable->getID(), "notarray");
	 		asm_vars.push_back(temp);
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
		 	
		 	//array	 		
	 		SymbolInfo * temp = new SymbolInfo($1->getName()+symbolTable->getID(), $3->getName());
	 		asm_vars.push_back(temp);	
 		  }
 		  ;
 		  
statements : statement
	   {
	   	$$ = assignProduction($1->getName()+"\n", "statement", "statements", lg);
	   	$$->code += $1->code;
	   	
	   }
	   | statements statement
	   {
	   	$$ = assignProduction($1->getName()+$2->getName()+"\n", "statements statement", "statements", lg);
	    	$$->code += $1->code+$2->code;
	   }
	   ;
	   
statement : var_declaration
	  {
	 	$$ = assignProduction($1->getName(), "var_declaration", "statement", lg);
	 	$$->code = "";
	  }
	  | expression_statement
	  {
	 	$$ = assignProduction($1->getName(), "expression_statement", "statement", lg);
	 	$$->code += $1->code;
	 	//cout << $$->code;
	  }
	  | compound_statement
	  {
	 	$$ = assignProduction($1->getName(), "compound_statement", "statement", lg);
	 	$$->code += $1->code;
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

		$$->code += $3->code;		
		char *label=newLabel();
		$$->code+="mov ax, "+$3->symbol+"\n";
		$$->code+="cmp ax, 0\n";
		$$->code+="je "+string(label)+"\n";
		$$->code+=$5->code;
		$$->code+=string(label)+":\n";	 	
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
	 	
	 	string code_ = "mov ax, " + $3->getName()+symbolTable->getID() + "\n";
	 	code_ += "mov print_var, ax\n";
	 	code_ += "call print\n";
	 	$$->code += code_;
	 	
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
				$$ -> code += $1->code;
				
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
	 	
	 	$$->symbol = $1->getName()+symbolTable->getID();
	 	$$->setVarType("notarray");
	 	
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
	 	
	 	$$->symbol = $1->getName()+symbolTable->getID()+"["+$3->getName()+"]";
	 	$$->code=$3->code+"mov bx, " +$3->symbol +"\nadd bx, bx\n";
	  	$$->setVarType("array");
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
 	   	
 	   	$$->code += $1->code;
 	   	$$->symbol = $1->symbol;
 	   	
 	   	
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
	   	
	   	$$->code = $3->code + $1->code;
		$$->code+="mov ax, "+$3->symbol+"\n";
		
		if(!$1->getVarType().compare("array"))
		{
			$$->code+= "mov  "+$1->symbol+"[bx], ax\n";
		}
		else
		{
			$$->code+= "mov "+$1->symbol+", ax\n";
		}
	   	
	   }
	   ;
			
logic_expression : rel_expression 	
		 {
		 	$$ = assignProduction($1->getName(), "rel_expression", "logic_expression", lg);
		 	$$ -> setDataType($1->getDataType());
		 	
		  	$$ -> code += $1 -> code;
		  	$$ -> symbol = $1 -> symbol;			 	
		 }
		 | rel_expression LOGICOP rel_expression 	
		 {
		 	string type = "rel_expression LOGICOP rel_expression";
			$$ = assignProduction($1->getName()+$2->getName()+$3->getName(), type , "logic_expression", lg);
			
			if(!$1->getDataType().compare("VOID") || !$3->getDataType().compare("VOID")) 
				$$ -> setDataType("VOID");
			else $$ -> setDataType("INT");
			
			$$->code += $1->code+$3->code;
			char * temp = newTemp();
			$$->symbol = string(temp);
			
			$$->code += "mov ax, " + $3->symbol + "\n";
			$$->code += "mov bx, " + $1->symbol +"\n";
			char * label1 = newLabel();
			char * label2 = newLabel();
			
			if(!$2->getName().compare("&&"))
			{
				//see if anyone is zero, jump to label1 if so
				$$->code += "cmp ax, 0\nje "+string(label1)+"\n";
				$$->code += "cmp bx, 0\nje "+string(label1)+"\n";

				//both non zero, set 1 and jump to label2...skipping label1
				$$->code += "mov ax, 1\n";
				$$->code += "mov " + string(temp) + ", ax\n";
				$$->code += "jmp " + string(label2) + "\n";
				
				//label1: set temp var = 0
				$$->code += string(label1) + ":\n";
				$$->code += "mov ax, 0\n";
				$$->code += "mov " + string(temp) + ", ax\n";
				
				$$->code += string(label2) + ":\n";
			}
			else //OR
			{
				//see if anyone is 1, jump to label1 if so
				$$->code += "cmp ax, 1\nje "+string(label1)+"\n";
				$$->code += "cmp bx, 1\nje "+string(label1)+"\n";

				//both zero, set 0 and jump to label2...skipping label1
				$$->code += "mov ax, 0\n";
				$$->code += "mov " + string(temp) + ", ax\n";
				$$->code += "jmp " + string(label2) + "\n";
				
				//label1: set temp var = 1
				$$->code += string(label1) + ":\n";
				$$->code += "mov ax, 1\n";
				$$->code += "mov " + string(temp) + ", ax\n";
				
				$$->code += string(label2) + ":\n";				
			}
		 }
		 ;
			
rel_expression	: simple_expression 
		{
			$$ = assignProduction($1->getName(), "simple_expression", "rel_expression", lg);
			$$ -> setDataType($1->getDataType());
		  	
		  	$$ -> code += $1 -> code;
		  	$$ -> symbol = $1 -> symbol;	
		  	//cout << $1->code;		
		}
		| simple_expression RELOP simple_expression	
		{
			string type = "simple_expression RELOP simple_expression";
			$$ = assignProduction($1->getName()+$2->getName()+$3->getName(), type , "rel_expression", lg);
			
			if(!$1->getDataType().compare("VOID") || !$3->getDataType().compare("VOID")) 
				$$ -> setDataType("VOID");
			else $$ -> setDataType("INT");
	
			$$->code += $1->code+$3->code;
			$$->code+="mov ax, " + $1->symbol+"\n";
			$$->code+="cmp ax, " + $3->symbol+"\n";
			char *temp=newTemp();
			char *label1=newLabel();
			char *label2=newLabel();
			
			if(!$2->getName().compare("<")){
				$$->code+="jl " + string(label1)+"\n";
			}
			else if(!$2->getName().compare("<=")){
				$$->code+="jle " + string(label1)+"\n";
			}
			else if(!$2->getName().compare(">")){
				$$->code+="jg " + string(label1)+"\n";			
			}
			else if(!$2->getName().compare(">=")){
				$$->code+="jge " + string(label1)+"\n";			
			}
			else if(!$2->getName().compare("==")){
				$$->code+="je " + string(label1)+"\n";			
			}
			else{
				$$->code+="jne " + string(label1)+"\n";			
			}
			
			$$->code+="mov "+string(temp) +", 0\n";
			$$->code+="jmp "+string(label2) +"\n";
			$$->code+=string(label1)+":\nmov "+string(temp)+", 1\n";
			$$->code+=string(label2)+":\n";
			$$->symbol = string(temp);			
		}
		;
				
simple_expression : term 
		  {
		  	$$ = assignProduction($1->getName(), "term", "simple_expression", lg);
		  	$$ -> setDataType($1->getDataType());
		  	
		  	$$ -> code += $1 -> code;
		  	$$ -> symbol = $1 -> symbol;
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
			
			$$->code += $1->code+$3->code;
			$$->code += "mov ax, " + $1->symbol + "\n";
			if(!$2->getName().compare("+"))
				$$->code += "add ax, " + $3->symbol+ "\n";
			else 
				$$->code += "sub ax, " + $3->symbol+ "\n";
			char * temp = newTemp();
			$$ ->code += "mov " + string(temp)+", ax\n";
			
			$$ -> symbol = string(temp);
			//cout << $$->code;
		  }
		  ;
					
term :	unary_expression
     {
     	$$ = assignProduction($1->getName(), "unary_expression", "term", lg);
     	$$ -> setDataType($1->getDataType());
     	
     	$$-> code +=  $1->code;
     	$$->symbol = $1->symbol;
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
	
	$$->code += $1->code+$3->code;
	$$->code += "mov ax, "+ $1->symbol+"\n";
	$$->code += "mov bx, "+ $3->symbol+"\n";
	char *temp=newTemp();
	if(!$2->getName().compare("*")){
		$$->code += "mul bx\n";
		$$->code += "mov "+ string(temp) + ", ax\n";
	}
	else if(!$2->getName().compare("/")){
		// clear dx, perform 'div bx' and mov ax to temp
		$$->code += "xor dx, dx\n";
		$$->code += "div bx\n";
		$$->code += "mov "+ string(temp) + ", ax\n";
	}
	else if(!$2->getName().compare("%")){
		// clear dx, perform 'div bx' and mov dx to temp
		$$->code += "xor dx, dx\n";
		$$->code += "div bx\n";
		$$->code += "mov "+ string(temp) + ", dx\n";		
	}
	$$->symbol = string(temp);
	//cout << $$->code ;
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
		 	
		 	$$->code += $2->code;
		 	$$->symbol = $2->symbol;
		 	
		 	//negate if -ve
		 	if(!$1->getName().compare("-"))
		 	{
			 	char *temp=newTemp();
				$$->code="mov ax, " + $2->symbol + "\n";
				$$->code+="neg ax\n";
				$$->code+="mov "+string(temp)+", ax";
				$$->symbol = string(temp);		 		
			}
			
		 }
		 | NOT unary_expression 
		 {
		 	$$ = assignProduction("!"+$2->getName(), "NOT unary_expression", "unary_expression", lg);
		 	if(!$2->getDataType().compare("VOID"))
		 		 $$ -> setDataType("VOID");
		 	else $$ -> setDataType("INT");	
		 	
		 	$$ -> code += $2 -> code;
		 	char *temp=newTemp();
			$$->code="mov ax, " + $2->symbol + "\n";
			$$->code+="not ax\n";
			$$->code+="mov "+string(temp)+", ax";		
	
			$$->symbol = string(temp); 	
		 		
		 }		 
		 | factor 
		 {
		 	$$ = assignProduction($1->getName(), "factor", "unary_expression", lg);
			$$ -> setDataType($1->getDataType());	
			$$ -> code += $1->code;
			$$ -> symbol = $1 -> symbol;			
		 }
		 ;
	
factor	: variable 
	{
		$$ = assignProduction($1->getName(), "variable", "factor", lg);
		$$ -> setDataType($1->getDataType());	
		$$->symbol = $1->symbol;	
		$$->code = "";
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
		$$->symbol = $2->symbol;	
		$$->code += $2->code;	
		//cout << $2->code;	
	}	
	| CONST_INT 
	{
		$$ = assignProduction($1->getName(), "CONST_INT", "factor", lg);	
		value_type = "INT";	
		$$ -> setDataType("INT");	
		$$->symbol = $1->getName();
		$$->code = "";
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
		
		$$->code += "mov ax, "+$1->symbol+"\nadd ax, 1\nmov "+$1->symbol+", ax\n";
		$$->symbol = $1->symbol;
		
	}	
	| variable DECOP
	{
		$$ = assignProduction($1->getName()+"--", "variable DECOP", "factor", lg);
		$$ -> setDataType($1->getDataType());
		
		$$->code += "mov ax, "+$1->symbol+"\nsub ax, 1\nmov "+$1->symbol+", ax\n";
		$$->symbol = $1->symbol;
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
	ac = fopen("code.asm", "w");
	
	symbolTable = new SymbolTable(30);
	asm_vars.push_back(new SymbolInfo("print_var" ,"notarray"));

	
	yyin=fp;
	yyparse();
	
	
	symbolTable -> PrintInFile(lg);
	fprintf(lg, "\nTotal lines: %d\nTotal errors: %d\n\n", line_count, error_count);
	
	
	fclose(yyin);
	fclose(lg);
	fclose(err);
	
	
	return 0;
}

