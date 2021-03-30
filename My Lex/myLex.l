%option noyywrap yylineno

%{
#include<stdio.h>
#include<stdlib.h>
#include "symboltable.h"
#include<string>

int line_count = 1;
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
void indentifier()
{
	fprintf(token_fp, "<ID, %s> ", yytext);
	printf("Line no %d: Token <ID> Lexeme %s found\n\n", line_count, yytext);
}	
%}


AlphaNumeric [a-zA-Z0-9_]
Letter [a-zA-Z_]
Identifier {Letter}{AlphaNumeric}*


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
{Identifier}		{indentifier();}
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
