%option noyywrap yylineno
%x CHARSTATE
%x CHARTERMINATION

%{
#include<stdio.h>
#include<stdlib.h>
#include "symboltable.h"
#include<string>

int line_count = 1;
int error_count = 0;
char * temp_char;
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
	string str = *temp_char + "";
	fprintf(token_fp, "<CONST_CHAR, %s> ", temp_char);
	printf("Line no %d: Token <CONST_CHAR> Lexeme %s found\n\n", line_count, temp_char);
	if(symbolTable.Insert(str, "CONST_CHAR"))
		symbolTable.PrintAllScopes();
}	
%}


AlphaNumeric [a-zA-Z0-9_]
Letter [a-zA-Z_]
Identifier {Letter}{AlphaNumeric}*
Digit [0-9]
Number {Digit}+
DecimalPlaceFloat {Number}(\.{Number})(E[+-]?{Number})?
NumPowerFloat {Number}(E[+-]?{Number})
PointFloat \.{Number}(E[+-]?{Number})?
Float {DecimalPlaceFloat}|{NumPowerFloat}|{PointFloat}
SpChars (\\n)|(\\t)|(\\\\)|(\\')|(\\a)|(\\f)|(\\r)|(\\b)|(\\v)|(\\0)


%%
[ \t]+ {}
[\n] {line_count++;}
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



<CHARTERMINATION>'	{foundChar(); BEGIN INITIAL;}
<CHARSTATE>{SpChars}	{*temp_char = yytext[1]; BEGIN CHARTERMINATION;}
'			{BEGIN CHARSTATE;}
<CHARSTATE>.		{*temp_char = yytext[0]; BEGIN CHARTERMINATION; }
<CHARTERMINATION>.	{
				printf("Error at line no %d: Unterminated character \'%s\n", line_count, temp_char);
				error_count++; BEGIN INITIAL; 
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
