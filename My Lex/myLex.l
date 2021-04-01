%option noyywrap yylineno
%x MULCOMM
%x SINCOMM

%{
#include<stdio.h>
#include<stdlib.h>		
#include "symboltable.h"
#include<string>

int line_count = 1;
int st_ln = 0;
int error_count = 0;
char* temp;
FILE *token_fp = fopen("token.txt","w");
SymbolTable symbolTable(10);

void printkeywords(const char* str)
{	
	fprintf(token_fp,"<%s> ", str);
	printf("Line no %d: Token <%s> Lexeme %s found\n\n", line_count, str, yytext);
}
void printOperators(const char* str)
{
	fprintf(token_fp, "<%s, %s> ", str, yytext);
	printf("Line no %d: Token <%s> Lexeme %s found\n\n", line_count, str, yytext);
}
void valueFound(const char* str)
{
	fprintf(token_fp, "<%s, %s> ", str, yytext);
	printf("Line no %d: Token <%s> Lexeme %s found\n\n", line_count, str, yytext);
	if(symbolTable.Insert(yytext, str)) 
		symbolTable.PrintAllScopes();
}
void foundChar()
{
	char c = yytext[1];
	string str = "";
	str += c;
	fprintf(token_fp, "<CONST_CHAR, %c> ", c);
	printf("Line no %d: Token <CONST_CHAR> Lexeme %c found\n\n", line_count, c);
	if(symbolTable.Insert(str, "CONST_CHAR"))
		symbolTable.PrintAllScopes();
}	
void foundSpChar()
{
	char ct = yytext[1];
	string str = "";
	str += ct;
	ct = yytext[2];
	str += ct;
	const char *c = str.c_str();
	fprintf(token_fp, "<CONST_CHAR, %s> ", c);
	printf("Line no %d: Token <CONST_CHAR> Lexeme %s found\n\n", line_count, c);
	if(symbolTable.Insert(str, "CONST_CHAR"))
		symbolTable.PrintAllScopes();
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
notbslash [^\\]
Chars '{notbslash}'
SpChars (\\n)|(\\t)|(\\\\)|(\\')|(\\a)|(\\f)|(\\r)|(\\b)|(\\v)|(\\0)
UndefSP (\\.)
notsinqt [^']
notws [^ ^\t^\n^'^\r]
UntermChar ('){notws}*
MultiChar '{notsinqt}{notsinqt}+'
MultiLineComment /\*.*\*/

%%
[ \t]+ {}
[\n] {line_count++;}
(\r\n) {line_count++;}


"//"			{ BEGIN SINCOMM; }
<SINCOMM>[^\\^\r^\n]	{}
<SINCOMM>(\r\n)|\n		{ BEGIN INITIAL; }
<SINCOMM>(\\\n)|(\\\r\n)		{}
<SINCOMM>\\[^\r^\n]	{ BEGIN INITIAL; }
<SINCOMM><<EOF>>	{ BEGIN INITIAL; }


"/*"			{ st_ln = line_count; BEGIN MULCOMM; }
<MULCOMM>(\r\n)|\n	{ line_count++; }
<MULCOMM>"*/"		{ BEGIN INITIAL; }
<MULCOMM><<EOF>>	{
				cout << "Error at line no "<< st_ln << ": ";
				cout << "Unterminated comment" << endl << endl;
				error_count++;
				BEGIN INITIAL;
			}
<MULCOMM>.		{}



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
(\{)			{printOperators("LCURL"); symbolTable.EnterScope();}
(\})			{printOperators("RCURL"); symbolTable.ExitScope();}
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
					cout << "Multi character constant error: " << yytext << endl << endl;
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
	fclose(yyin);
	fclose(stdout);
	fclose(token_fp);
	return 0;
}
