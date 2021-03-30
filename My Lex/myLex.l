%option noyywrap

%{
#include<stdio.h>
#include<stdlib.h>
%}

AlphaNumeric [a-zA-Z0-9]
Letter [a-zA-Z]

%%
[ \t\n]+ {}
(if) 			{printf("%s matched as a keyword\n", yytext);}
(for) 			{printf("%s matched as a keyword\n", yytext);}
(do) 			{printf("%s matched as a keyword\n", yytext);}
(int) 			{printf("%s matched as a keyword\n", yytext);}
(float) 		{printf("%s matched as a keyword\n", yytext);}
(void) 		{printf("%s matched as a keyword\n", yytext);}
(switch) 		{printf("%s matched as a keyword\n", yytext);}
(default) 		{printf("%s matched as a keyword\n", yytext);}
(else)			{printf("%s matched as a keyword\n", yytext);}
(while) 		{printf("%s matched as a keyword\n", yytext);}
(break) 		{printf("%s matched as a keyword\n", yytext);}
(char) 		{printf("%s matched as a keyword\n", yytext);}
(double) 		{printf("%s matched as a keyword\n", yytext);}
(return) 		{printf("%s matched as a keyword\n", yytext);}
(case) 		{printf("%s matched as a keyword\n", yytext);}
(continue) 		{printf("%s matched as a keyword\n", yytext);}
[A-Z]{3}		{printf("%s matched in [A-Z]{3} rule\n",yytext);}
a* 			{printf("%s matched in a* rule\n",yytext);}
A* 			{printf("%s matched in A* rule\n",yytext);}
{AlphaNumeric}+ 	{printf("%s matched in Alphanumeric rule\n",yytext);}
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
	return 0;
}
