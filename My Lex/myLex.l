%option noyywrap yylineno

%{
#include<stdio.h>
#include<stdlib.h>
#include "symboltable.h"

int line_count = 1;
FILE *log_fp=fopen("log.txt","w");
FILE *token_fp=fopen("token.txt","w");
SymbolTable symbolTable(10);
%}


AlphaNumeric [a-zA-Z0-9]
Letter [a-zA-Z]


%%
[ \t]+ {}
[\n] {line_count++;}
(if) 			{fprintf(token_fp, "<IF> ");}
(for) 			{fprintf(token_fp, "<FOR> ");}
(do) 			{fprintf(token_fp, "<DO> ");}
(int) 			{fprintf(token_fp, "<INT> ");}
(float) 		{fprintf(token_fp, "<FLOAT> ");}
(void) 		{fprintf(token_fp, "<VOID> ");}
(switch) 		{fprintf(token_fp, "<SWITCH> ");}
(default) 		{fprintf(token_fp, "<DEFAULT> ");}
(else)			{fprintf(token_fp, "<ELSE> ");}
(while) 		{fprintf(token_fp, "<WHILE> ");}
(break) 		{fprintf(token_fp, "<BREAK> ");}
(char) 		{fprintf(token_fp, "<CHAR> ");}
(double) 		{fprintf(token_fp, "<DOUBLE> ");}
(return) 		{fprintf(token_fp, "<RETURN> ");}
(case) 		{fprintf(token_fp, "<CASE> ");}
(continue) 		{fprintf(token_fp, "<CONTINUE> ");}
(\+\+)			{fprintf(token_fp, "<INCOP, %s> ", yytext);}
(\-\-)			{fprintf(token_fp, "<INCOP, %s> ", yytext);}
(\|\|)			{fprintf(token_fp, "<LOGICOP, %s> ", yytext);}
(&&)			{fprintf(token_fp, "<LOGICOP, %s> ", yytext);}
(\<=)			{fprintf(token_fp, "<RELOP, %s> ", yytext);}
(\>=)			{fprintf(token_fp, "<RELOP, %s> ", yytext);}
(==)			{fprintf(token_fp, "<RELOP, %s> ", yytext);}
(!=)			{fprintf(token_fp, "<RELOP, %s> ", yytext);}
(\<)			{fprintf(token_fp, "<RELOP, %s> ", yytext);}
(\>)			{fprintf(token_fp, "<RELOP, %s> ", yytext);}
(\+)			{fprintf(token_fp, "<ADDOP, %s> ", yytext);}
(\-)			{fprintf(token_fp, "<ADDOP, %s> ", yytext);}
(\*)			{fprintf(token_fp, "<MULOP, %s> ", yytext);}
(\/)			{fprintf(token_fp, "<MULOP, %s> ", yytext);}
(%)			{fprintf(token_fp, "<MULOP, %s> ", yytext);}
(=)			{fprintf(token_fp, "<ASSIGNOP, %s> ", yytext);}
(!)			{fprintf(token_fp, "<NOT, %s> ", yytext);}
(\()			{fprintf(token_fp, "<LPAREN, %s> ", yytext);}
(\))			{fprintf(token_fp, "<RPAREN, %s> ", yytext);}
(\{)			{fprintf(token_fp, "<LCURL, %s> ", yytext); symbolTable.EnterScope();}
(\})			{fprintf(token_fp, "<RCURL, %s> ", yytext); symbolTable.ExitScope();}
(\[)			{fprintf(token_fp, "<LTHIRD, %s> ", yytext);}
(\])			{fprintf(token_fp, "<RTHIRD, %s> ", yytext);}
(,)			{fprintf(token_fp, "<COMMA, %s> ", yytext);}
(;)			{fprintf(token_fp, "<SEMICOLON, %s> ", yytext);}

{Letter}{AlphaNumeric}+ 	{printf("%s matched as Identifier rule\n",yytext);}
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
	

	yyin= fin;
	yylex();
	fclose(yyin);
	fclose(log_fp);
	fclose(token_fp);
	return 0;
}
