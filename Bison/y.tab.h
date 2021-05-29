/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NEWLINE = 258,
    IF = 259,
    FOR = 260,
    DO = 261,
    INT = 262,
    FLOAT = 263,
    VOID = 264,
    SWITCH = 265,
    DEFAULT = 266,
    ELSE = 267,
    WHILE = 268,
    BREAK = 269,
    CHAR = 270,
    DOUBLE = 271,
    RETURN = 272,
    CASE = 273,
    CONTINUE = 274,
    PRINTLN = 275,
    INCOP = 276,
    DECOP = 277,
    ASSIGNOP = 278,
    NOT = 279,
    LPAREN = 280,
    RPAREN = 281,
    LCURL = 282,
    RCURL = 283,
    LTHIRD = 284,
    RTHIRD = 285,
    COMMA = 286,
    SEMICOLON = 287,
    LOGICOP = 288,
    RELOP = 289,
    ADDOP = 290,
    MULOP = 291,
    CONST_FLOAT = 292,
    CONST_INT = 293,
    ID = 294,
    FAKE_ELSE = 295
  };
#endif
/* Tokens.  */
#define NEWLINE 258
#define IF 259
#define FOR 260
#define DO 261
#define INT 262
#define FLOAT 263
#define VOID 264
#define SWITCH 265
#define DEFAULT 266
#define ELSE 267
#define WHILE 268
#define BREAK 269
#define CHAR 270
#define DOUBLE 271
#define RETURN 272
#define CASE 273
#define CONTINUE 274
#define PRINTLN 275
#define INCOP 276
#define DECOP 277
#define ASSIGNOP 278
#define NOT 279
#define LPAREN 280
#define RPAREN 281
#define LCURL 282
#define RCURL 283
#define LTHIRD 284
#define RTHIRD 285
#define COMMA 286
#define SEMICOLON 287
#define LOGICOP 288
#define RELOP 289
#define ADDOP 290
#define MULOP 291
#define CONST_FLOAT 292
#define CONST_INT 293
#define ID 294
#define FAKE_ELSE 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 81 "parser.y"
SymbolInfo* si; string* str;

#line 140 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
