%option noyywrap yylineno
%x MULCOMM
%x SINCOMM
%x STRINGSTATE

%{

#include<iostream>
using namespace std;
#include<stdlib.h>
#include<string.h>
#include "symboltable.h"
#include "y.tab.h"


using namespace std;

extern YYSTYPE yylval;
void yyerror(char *);
int line_count = 1;
int error_count = 0;

int st_ln = 0;
int total_lines;
string temp_str = "\"";
string string_token = "";
string comment = "";



string lexString()
{
    string s = "";
    for (int i = 0; i < yyleng; i++) {
        s += yytext[i];
    }
    return s;
}

void printOperators(string s){}
void valueFound(string s){}
void foundChar()
{
	char ct = yytext[1];
	string str = "\'";
	str += ct;
	str += "\'";
	const char* c = str.c_str();
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
	comment = "";
}

//parser communicating functions
void sendValue(string token)
{
	SymbolInfo * si = new SymbolInfo(lexString(), token);
	yylval.si = si;
}
%}

WhiteSpace [ \t\n]
AlphaNumeric [a-zA-Z0-9_]
Letter [a-zA-Z_]
Identifier {Letter}{AlphaNumeric}*
Digit [0-9]
Number {Digit}+
DecimalPlaceFloat {Number}(\.{Number})((e|E)[+-]?{Number})?
NumPowerFloat {Number}((e|E)[+-]?{Number})
PointFloat \.{Number}((e|E)[+-]?{Number})?
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
Ill1 ({Number}((e|E)[+-]?{Number})(\.+{Number}*)+)
Ill2 (\.{Number}((e|E)[+-]?{Number})(\.+{Number}*)+)
Ill3 ({Number}(\.{Number})((e|E)[+-]?{Number})(\.+{Number}*)+)
Illnum {Ill1}|{Ill2}|{Ill3}

%%
[ \t]+ {}
(\n)|(\r\n) {line_count++; return NEWLINE; }


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



(if) 		{ return IF;}
(for) 		{ return FOR;}
(do) 		{ return DO;}
(int) 		{ return INT;}
(float) 	{ return FLOAT;}
(void) 		{return VOID; }
(switch) 	{ return SWITCH;}
(default) 	{ return DEFAULT;}
(else)		{ return ELSE;}
(while) 	{ return WHILE;}
(break) 	{ return BREAK;}
(char) 		{ return CHAR;}
(double) 	{ return DOUBLE;}
(return) 	{ return RETURN;}
(case) 		{ return CASE;}
(continue) 	{ return CONTINUE;}
(printf)	{ return PRINTLN; }


(\+\+)			{return INCOP; }
(\-\-)			{return DECOP; }

(\|\|)			{sendValue("LOGICOP"); return LOGICOP; }
(&&)			{sendValue("LOGICOP"); return LOGICOP; }

(\<=)			{sendValue("RELOP"); return RELOP; }
(\>=)			{sendValue("RELOP"); return RELOP; }
(==)			{sendValue("RELOP"); return RELOP; }
(!=)			{sendValue("RELOP"); return RELOP; }
(\<)			{sendValue("RELOP"); return RELOP; }
(\>)			{sendValue("RELOP"); return RELOP; }

(\+)			{sendValue("ADDOP"); return ADDOP; }
(\-)			{sendValue("ADDOP"); return ADDOP; }

(\*)			{sendValue("MULOP"); return MULOP; }
(\/)			{sendValue("MULOP"); return MULOP; }
(%)			{sendValue("MULOP"); return MULOP; }

(=)			{return ASSIGNOP;}
(!)			{return NOT;}
(\()			{return LPAREN;}
(\))			{return RPAREN;}
(\{)			{return LCURL;}
(\})			{return RCURL;}
(\[)			{return LTHIRD;}
(\])			{return RTHIRD;}
(,)			{return COMMA;}
(;)			{return SEMICOLON;}

{Float}		{sendValue("CONST_FLOAT"); return CONST_FLOAT; }
{Number}		{sendValue("CONST_INT"); return CONST_INT; }
{Identifier}		{sendValue("ID"); return ID; }




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
