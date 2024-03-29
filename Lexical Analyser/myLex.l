%option noyywrap yylineno
%x MULCOMM
%x SINCOMM
%x STRINGSTATE

%{
#include<stdio.h>
#include<stdlib.h>		
#include "symboltable.h"
#include<string>

int line_count = 1;
int st_ln = 0;
int error_count = 0;
int total_lines;
string temp_str = "\"";
string string_token = "";
string comment = "";
FILE *token_fp = fopen("token.txt","w");
SymbolTable* symbolTable = new SymbolTable(7);

string lexString()
{
    string s = "";
    for (int i = 0; i < yyleng; i++) {
        s += yytext[i];
    }
    return s;
}

void printkeywords(const char* str)
{	
	fprintf(token_fp,"<%s> ", str);
	printf("\nLine no %d: Token <%s> Lexeme %s found\n", line_count, str, yytext);
}
void printOperators(const char* str)
{
	fprintf(token_fp, "<%s, %s> ", str, yytext);
	printf("\nLine no %d: Token <%s> Lexeme %s found\n", line_count, str, yytext);
}
void valueFound(const char* str)
{
	fprintf(token_fp, "<%s, %s> ", str, yytext);
	printf("\nLine no %d: Token <%s> Lexeme %s found\n", line_count, str, yytext);
	if(symbolTable->Insert(yytext, str)) 
		symbolTable->PrintAllScopes();
}
void foundChar()
{
	char ct = yytext[1];
	string str = "\'";
	str += ct;
	str += "\'";
	const char* c = str.c_str();
	printf("\nLine no %d: Token <CONST_CHAR> Lexeme %s found --> <CHAR_CONST, %c> \n", line_count, c, ct);
	fprintf(token_fp, "<CHAR_CONST, %c> ", ct);
	if(symbolTable->Insert(str, "CONST_CHAR"))
		symbolTable->PrintAllScopes();
}
char genSpChar(string str)
{
    if(!str.compare("\\n")) return '\n';
    if(!str.compare("\\t")) return '\t';
    if(!str.compare("\\a")) return '\a';
    if(!str.compare("\\f")) return '\f';
    if(!str.compare("\\b")) return '\b';
    if(!str.compare("\\r")) return '\r';
    if(!str.compare("\\0")) return '\0';
    if(!str.compare("\\v")) return '\v';
    if(!str.compare("\\\'")) return '\'';
    if(!str.compare("\\\\")) return '\\';
    if(!str.compare("\\\"")) return '\"';
    return 's';
}	
void foundSpChar()
{
	char ct = yytext[1];
	string str = "";
	str += ct;
	ct = yytext[2];
	str += ct;
	string str_stable = "\'" + str + "\'";
	const char *c = str_stable.c_str();
	char ctkn = genSpChar(str);
	fprintf(token_fp, "<CHAR_CONST, %c> ", ctkn);
	printf("\nLine no %d: Token <CONST_CHAR> Lexeme %s ", line_count, c);
	printf("found --> <CHAR_CONST, %c> \n", ctkn);
	if(symbolTable->Insert(str_stable, "CONST_CHAR"))
		symbolTable->PrintAllScopes();
}
void appendString()
{
	char c = yytext[0];
	temp_str += c;
	string_token += c;
}
void foundString()
{
	temp_str += "\"";
	const char* c = temp_str.c_str();
	const char* ct = string_token.c_str();
	fprintf(token_fp, "<STRING, \"%s\"> ", ct);
	printf("\nLine no %d: Token <STRING> Lexeme %s ", st_ln, c);
	printf("found --> <STRING, \"%s\"> \n", ct);
	temp_str = "\"";
	string_token = "";
}
void str_term_error()
{
	cout << "Error at line no " << st_ln <<": ";
	cout << "Unterminated String " << temp_str << endl << endl;
	error_count++;
	temp_str = "\"";
	string_token = "";
}
void handleSpString()
{
	char c = yytext[0];
	string str = "";
	str += c;
	c = yytext[1];
	str += c;
	c = genSpChar(str);
	temp_str += str;
	string_token += c;
}
void commentFound()
{
	const char * c = comment.c_str();
	printf("\nLine no %d: Token <COMMENT> Lexeme %s found\n", st_ln, c);
	comment = "";
}
%}

WhiteSpace [ \t\n]
AlphaNumeric [a-zA-Z0-9_]
Letter [a-zA-Z_]
Identifier {Letter}{AlphaNumeric}*
Digit [0-9]
Number {Digit}+
DecimalPlaceFloat {Number}(\.{Number})(E[+-]?{Number})?
NumPowerFloat {Number}(E[+-]?{Number})
PointFloat \.{Number}(E[+-]?{Number})?
Float {DecimalPlaceFloat}|{NumPowerFloat}|{PointFloat}
notbslash [^\\']
Chars '{notbslash}'
SpChars (\\n)|(\\t)|(\\\\)|(\\')|(\\\")|(\\a)|(\\f)|(\\r)|(\\b)|(\\v)|(\\0)
UndefSP (\\.)
notsinqt [^'\r\n]
notws [^ ^\t^\n^'^\r]
UntermChar ('{notws}*)|('\\')
MultiChar '{notsinqt}{notsinqt}+'
MultiLineComment /\*.*\*/
TooManyDp ((\.[0-9]*)((\.[0-9]*)+)|(([0-9]*)\.)(([0-9]*)\.)(([0-9]*\.?)+))([E][+-]?[0-9]*)?
Unrecog ([^a-zA-Z_0-9\n\t\\\'\a\f\r\b\v\0\"+\-*\/%<>=!&|(){}\[\],;])+
EmptyChar ''
InvalID {Number}+{Letter}+
Ill1 ({Number}(E[+-]?{Number})(\.+{Number}*)+)
Ill2 (\.{Number}(E[+-]?{Number})(\.+{Number}*)+)
Ill3 ({Number}(\.{Number})(E[+-]?{Number})(\.+{Number}*)+)
Illnum {Ill1}|{Ill2}|{Ill3}

%%
[ \t]+ {}
(\n)|(\r\n) {line_count++;}


"//"				{ st_ln = line_count; comment += lexString(); BEGIN SINCOMM; }
<SINCOMM>[^\\^\r^\n]		{ comment += lexString(); }
<SINCOMM>(\r\n)|\n		{ line_count++; commentFound(); BEGIN INITIAL; }
<SINCOMM>(\\\r\n)|(\\\n)	{ line_count++; comment += lexString(); }
<SINCOMM>\\[^\r^\n]		{ comment += lexString();  }
<SINCOMM><<EOF>>		{ commentFound(); BEGIN INITIAL; }


"/*"			{ st_ln = line_count; comment += lexString(); BEGIN MULCOMM; }
<MULCOMM>(\r\n)|\n	{ comment += lexString(); line_count++; }
<MULCOMM>"*/"		{ comment += lexString(); commentFound(); BEGIN INITIAL; }
<MULCOMM><<EOF>>	{
				cout << "Error at line no "<< st_ln << ": ";
				cout << "Unterminated Comment " << comment << endl << endl;
				error_count++;
				comment = "";
				BEGIN INITIAL;
			}
<MULCOMM>.		{ comment += lexString(); }



(if) 		{printkeywords("IF");}
(for) 		{printkeywords("FOR");}
(do) 		{printkeywords("DO");}
(int) 		{printkeywords("INT");}
(float) 	{printkeywords("FLOAT");}
(void) 		{printkeywords("VOID");}
(switch) 	{printkeywords("SWITCH");}
(default) 	{printkeywords("DEFAULT");}
(else)		{printkeywords("ELSE");}
(while) 	{printkeywords("WHILE");}
(break) 	{printkeywords("BREAK");}
(char) 		{printkeywords("CHAR");}
(double) 	{printkeywords("DOUBLE");}
(return) 	{printkeywords("RETURN");}
(case) 		{printkeywords("CASE");}
(continue) 	{printkeywords("CONTINUE");}


(\+\+)			{printOperators("INCOP");}
(\-\-)			{printOperators("INCOP");}
(\|\|)			{printOperators("LOGICOP");}
(&&)			{printOperators("LOGICOP");}
(\<=)			{printOperators("RELOP");}
(\>=)			{printOperators("RELOP");}
(==)			{printOperators("RELOP");}
(!=)			{printOperators("RELOP");}
(\<)			{printOperators("RELOP");}
(\>)			{printOperators("RELOP");}
(\+)			{printOperators("ADDOP");}
(\-)			{printOperators("ADDOP");}
(\*)			{printOperators("MULOP");}
(\/)			{printOperators("MULOP");}
(%)			{printOperators("MULOP");}
(=)			{printOperators("ASSIGNOP");}
(!)			{printOperators("NOT");}
(\()			{printOperators("LPAREN");}
(\))			{printOperators("RPAREN");}
(\{)			{printOperators("LCURL"); symbolTable->EnterScope();}
(\})			{printOperators("RCURL"); symbolTable->ExitScope();}
(\[)			{printOperators("LTHIRD");}
(\])			{printOperators("RTHIRD");}
(,)			{printOperators("COMMA");}
(;)			{printOperators("SEMICOLON");}
{Float}			{valueFound("CONST_FLOAT");}
{Number}		{valueFound("CONST_INT");}
{Identifier}		{valueFound("ID");}




{Chars}			{foundChar();}
('{SpChars}')			{foundSpChar();}
{UntermChar}			{
					cout << "Error at line no "<< line_count << ": ";
					cout << "Unterminated character " << yytext << endl << endl;
					error_count++;
				}
{MultiChar}			{
					cout << "Error at line no "<< line_count << ": ";
					cout << "Multi character constant error " << yytext << endl << endl;
					error_count++;
				}
				



\"				{ st_ln = line_count; BEGIN STRINGSTATE; }
<STRINGSTATE>\"		{ foundString(); BEGIN INITIAL; }
<STRINGSTATE>{SpChars}		{ handleSpString() ;}
<STRINGSTATE>(\\\r\n)|(\\\n) 	{ line_count++; temp_str += lexString(); }
<STRINGSTATE>\\[^\r\n]		{ 
					str_term_error();
					BEGIN INITIAL;
				}
<STRINGSTATE>(\r\n)|(\n)	{	
					str_term_error(); line_count++; BEGIN INITIAL;
				}	
<STRINGSTATE>[^\r\n\\]		{ appendString(); }
<STRINGSTATE><<EOF>>		{ str_term_error(); 	BEGIN INITIAL; }




{TooManyDp}			{
					cout << "Error at line no "<< line_count << ": ";
					cout << "Too many decimal points " << yytext << endl << endl;
					error_count++;
				}
{Unrecog}			{
					cout << "Error at line no "<< line_count << ": ";
					cout << "Unrecognized character " << yytext << endl << endl;
					error_count++;
				}				
{EmptyChar}			{
					cout << "Error at line no "<< line_count << ": ";
					cout << "Empty character constant error " << yytext << endl << endl;
					error_count++;
				}
{InvalID}			{
					cout << "Error at line no "<< line_count << ": Invalid prefix on ID";
					cout << " or invalid suffix on Number " << yytext << endl << endl;
					error_count++;
				}
{Illnum}			{
					cout << "Error at line no "<< line_count << ": ";
					cout << "Ill formed number " << yytext << endl << endl;
					error_count++;
				}
											
. 			{printf("Mysterious character %s found\n",yytext);}
%%
int main(int argc,char *argv[]){
	
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	freopen("log.txt","w",stdout);

	
	yyin= fin;
	yylex();
	
	symbolTable -> PrintAllScopes();
	cout << "\nTotal lines: " << line_count << endl;
	cout << "Total errors: " << error_count << endl;
	
	fclose(yyin);
	fclose(stdout);
	fclose(token_fp);
	delete symbolTable;
	return 0;
}
