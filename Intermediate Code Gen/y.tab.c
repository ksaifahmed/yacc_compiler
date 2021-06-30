/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

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


#line 190 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
#line 127 "parser.y" /* yacc.c:355  */
SymbolInfo* si; string* str;

#line 313 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 330 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   146

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  70
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  124

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   147,   147,   168,   173,   180,   185,   190,   198,   197,
     224,   223,   250,   249,   302,   301,   341,   357,   362,   371,
     378,   378,   385,   385,   394,   409,   413,   417,   423,   436,
     452,   464,   480,   486,   493,   498,   504,   509,   529,   542,
     559,   575,   595,   602,   606,   614,   636,   660,   677,   713,
     721,   778,   787,   830,   838,   863,   871,   920,   944,   960,
     969,   976,  1018,  1026,  1034,  1040,  1051,  1063,  1068,  1073,
    1079
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NEWLINE", "IF", "FOR", "DO", "INT",
  "FLOAT", "VOID", "SWITCH", "DEFAULT", "ELSE", "WHILE", "BREAK", "CHAR",
  "DOUBLE", "RETURN", "CASE", "CONTINUE", "PRINTLN", "INCOP", "DECOP",
  "ASSIGNOP", "NOT", "LPAREN", "RPAREN", "LCURL", "RCURL", "LTHIRD",
  "RTHIRD", "COMMA", "SEMICOLON", "LOGICOP", "RELOP", "ADDOP", "MULOP",
  "CONST_FLOAT", "CONST_INT", "ID", "FAKE_ELSE", "$accept", "start",
  "program", "unit", "func_declaration", "$@1", "$@2", "func_definition",
  "$@3", "$@4", "parameter_list", "compound_statement", "$@5", "$@6",
  "var_declaration", "type_specifier", "declaration_list", "statements",
  "statement", "expression_statement", "variable", "expression",
  "logic_expression", "rel_expression", "simple_expression", "term",
  "unary_expression", "factor", "argument_list", "arguments", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295
};
# endif

#define YYPACT_NINF -70

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-70)))

#define YYTABLE_NINF -23

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      43,   -70,   -70,   -70,    12,    43,   -70,   -70,   -70,   -70,
     -16,   -70,   -70,    -3,    14,     6,    -4,   -11,   -70,     9,
     -10,     0,    27,    24,    33,    32,    58,    43,   -70,   -70,
      42,   -70,    59,   -70,    38,    32,    47,    60,   100,    65,
     -70,   -70,   -70,   -70,    69,    70,    72,    91,    73,    91,
      91,   -70,    91,   -70,   -70,     8,   -70,   -70,    67,    64,
     -70,   -70,    53,    68,   -70,    77,    44,    75,   -70,   -70,
     -70,    91,    23,    91,    80,    79,    61,   -70,    88,   -70,
      91,    91,    90,   -70,   -70,   -70,   -70,    91,   -70,    91,
      91,    91,    91,    95,    23,    96,   -70,    97,   -70,   -70,
     105,   102,   104,   -70,   -70,   101,    75,   -70,   100,    91,
     100,   108,   -70,    91,   -70,   129,   116,   -70,   -70,   -70,
     100,   100,   -70,   -70
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    25,    26,    27,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    30,     0,     0,     0,     0,    24,    10,
       0,    19,     0,    28,     0,     0,     8,     0,    18,    31,
       0,    11,    20,    15,     0,     0,    17,     0,     0,     0,
       9,    13,    16,    29,     0,     0,     0,     0,     0,     0,
       0,    43,     0,    64,    63,    45,    36,    34,     0,     0,
      32,    35,    60,     0,    47,    49,    51,    53,    55,    59,
      23,     0,     0,     0,     0,     0,    60,    58,     0,    57,
      68,     0,    30,    21,    33,    65,    66,     0,    44,     0,
       0,     0,     0,     0,     0,     0,    42,     0,    62,    70,
       0,    67,     0,    48,    50,    52,    54,    56,     0,     0,
       0,     0,    61,     0,    46,    38,     0,    40,    41,    69,
       0,     0,    39,    37
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -70,   -70,   -70,   138,   -70,   -70,   -70,   -70,   -70,   -70,
     -70,   -15,   -70,   -70,    19,     2,   -70,   -70,   -54,   -64,
     -49,   -46,   -69,    55,    56,    54,   -43,   -70,   -70,   -70
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    34,    24,     8,    35,    25,
      20,    56,    38,    39,    57,    58,    14,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,   100,   101
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      76,    74,    10,    76,    78,    84,    77,    10,    94,    79,
      33,    99,    11,     1,     2,     3,    26,    21,   103,     9,
      41,    27,    15,    13,     9,    93,    16,    95,    23,    36,
     109,    76,    19,    80,    22,   102,   -14,    81,    76,    28,
      76,    76,    76,    76,   119,    17,    18,    49,    50,   107,
       1,     2,     3,    30,   115,    51,   117,    29,    52,    32,
      53,    54,    55,   116,    76,    31,   122,   123,    44,    45,
      40,     1,     2,     3,    85,    86,    87,    46,    90,    91,
      37,    47,    85,    86,    48,   -12,    42,   -22,    49,    50,
      43,    32,    83,    70,    71,    72,    51,    73,    75,    52,
      88,    53,    54,    55,    44,    45,    82,     1,     2,     3,
      89,    92,    96,    46,    98,    49,    50,    47,    97,    16,
      48,   108,   110,   111,    49,    50,    52,    32,    53,    54,
      55,   112,    51,   113,   114,    52,    91,    53,    54,    55,
     118,   120,   121,    12,   104,   106,   105
};

static const yytype_uint8 yycheck[] =
{
      49,    47,     0,    52,    50,    59,    49,     5,    72,    52,
      25,    80,     0,     7,     8,     9,    26,    15,    87,     0,
      35,    31,    25,    39,     5,    71,    29,    73,    39,    27,
      94,    80,    26,    25,    38,    81,    27,    29,    87,    39,
      89,    90,    91,    92,   113,    31,    32,    24,    25,    92,
       7,     8,     9,    29,   108,    32,   110,    30,    35,    27,
      37,    38,    39,   109,   113,    32,   120,   121,     4,     5,
      32,     7,     8,     9,    21,    22,    23,    13,    34,    35,
      38,    17,    21,    22,    20,    27,    39,    28,    24,    25,
      30,    27,    28,    28,    25,    25,    32,    25,    25,    35,
      32,    37,    38,    39,     4,     5,    39,     7,     8,     9,
      33,    36,    32,    13,    26,    24,    25,    17,    39,    29,
      20,    26,    26,    26,    24,    25,    35,    27,    37,    38,
      39,    26,    32,    31,    30,    35,    35,    37,    38,    39,
      32,    12,    26,     5,    89,    91,    90
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,     8,     9,    42,    43,    44,    45,    48,    55,
      56,     0,    44,    39,    57,    25,    29,    31,    32,    26,
      51,    56,    38,    39,    47,    50,    26,    31,    39,    30,
      29,    32,    27,    52,    46,    49,    56,    38,    53,    54,
      32,    52,    39,    30,     4,     5,    13,    17,    20,    24,
      25,    32,    35,    37,    38,    39,    52,    55,    56,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      28,    25,    25,    25,    62,    25,    61,    67,    62,    67,
      25,    29,    39,    28,    59,    21,    22,    23,    32,    33,
      34,    35,    36,    62,    60,    62,    32,    39,    26,    63,
      69,    70,    62,    63,    64,    65,    66,    67,    26,    60,
      26,    26,    26,    31,    30,    59,    62,    59,    32,    63,
      12,    26,    59,    59
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    44,    46,    45,
      47,    45,    49,    48,    50,    48,    51,    51,    51,    51,
      53,    52,    54,    52,    55,    56,    56,    56,    57,    57,
      57,    57,    58,    58,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    60,    60,    61,    61,    62,    62,    63,
      63,    64,    64,    65,    65,    66,    66,    67,    67,    67,
      68,    68,    68,    68,    68,    68,    68,    69,    69,    70,
      70
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     0,     7,
       0,     6,     0,     7,     0,     6,     4,     3,     2,     1,
       0,     4,     0,     3,     3,     1,     1,     1,     3,     6,
       1,     4,     1,     2,     1,     1,     1,     7,     5,     7,
       5,     5,     3,     1,     2,     1,     4,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     2,     2,     1,
       1,     4,     3,     1,     1,     2,     2,     1,     0,     3,
       1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 148 "parser.y" /* yacc.c:1646  */
    {
		//$$ = assignProduction($1->getName(), "program", "start", lg);
		fprintf(lg, "Line %d: start : program\n\n", line_count); 
		
		if(error_count == 0){
			//headers
			fprintf(ac, ".model small\n.stack 100h\n"); 
		
			//data
			fprintf(ac, "%s\n", getDataAsm().c_str());
		
			//println() function
			fprintf(ac, ".code%s", PrintFunction().c_str()); 
		
			//codes
			fprintf(ac, "%s", (yyvsp[0].si)->code.c_str()); 
		}
	}
#line 1519 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 169 "parser.y" /* yacc.c:1646  */
    {
		(yyval.si) = assignProduction((yyvsp[-1].si)->getName()+(yyvsp[0].si)->getName()+"\n", "program unit", "program", lg);
	     	(yyval.si)->code += (yyvsp[-1].si)->code + (yyvsp[0].si)->code;
	}
#line 1528 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 174 "parser.y" /* yacc.c:1646  */
    {
		(yyval.si) = assignProduction((yyvsp[0].si)->getName()+"\n", "unit", "program", lg);
	     	(yyval.si)->code += (yyvsp[0].si)->code;		
	}
#line 1537 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 181 "parser.y" /* yacc.c:1646  */
    {
     	(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "var_declaration", "unit", lg);
     	(yyval.si)->code = "";     	     	
     }
#line 1546 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 186 "parser.y" /* yacc.c:1646  */
    {
     	(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "func_declaration", "unit", lg);
     	(yyval.si)->code += (yyvsp[0].si)->code;     	
     }
#line 1555 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 191 "parser.y" /* yacc.c:1646  */
    {
     	(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "func_definition", "unit", lg);
     	(yyval.si)->code += (yyvsp[0].si)->code;
     }
#line 1564 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 198 "parser.y" /* yacc.c:1646  */
    {
			SymbolInfo * sp = symbolTable -> Lookup((yyvsp[-3].si)->getName());
			if(sp!=NULL)
			{
				string f_name = (yyvsp[-3].si) -> getName();
				printError("Multiple declaration of "+f_name);
			}
		}
#line 1577 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 207 "parser.y" /* yacc.c:1646  */
    {
			string type = "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON";
			string name = (yyvsp[-6].si)->getName()+" "+(yyvsp[-5].si)->getName()+"("+(yyvsp[-3].si)->getName()+");";
			(yyval.si) = assignProduction(name, type, "func_declaration", lg);
			
			FunctionInfo f;
			for(int i=0; i<params.size(); i++)
			{
				SymbolInfo * s = params.at(i);
				f.insertParams(s->getName(), s->getType(), s->getDataType());
			}
			params.clear();
			symbolTable -> Insert((yyvsp[-5].si)->getName(), "ID", (yyvsp[-6].si)->getType(), "function", f);
			
			
		}
#line 1598 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 224 "parser.y" /* yacc.c:1646  */
    {
			SymbolInfo * sp = symbolTable -> Lookup((yyvsp[-2].si)->getName());
			if(sp!=NULL)
			{
				string f_name = (yyvsp[-2].si) -> getName();
				printError("Multiple declaration of "+f_name);
			}	
		}
#line 1611 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 233 "parser.y" /* yacc.c:1646  */
    {
			string type = "type_specifier ID LPAREN RPAREN SEMICOLON";
			string name = (yyvsp[-5].si)->getName()+" "+(yyvsp[-4].si)->getName()+"();";
			(yyval.si) = assignProduction(name, type, "func_declaration", lg);
			
			FunctionInfo f;
			for(int i=0; i<params.size(); i++)
			{
				SymbolInfo * s = params.at(i);
				f.insertParams(s->getName(), s->getType(), s->getDataType());
			}
			params.clear();
			symbolTable -> Insert((yyvsp[-4].si)->getName(), "ID", (yyvsp[-5].si)->getType(), "function", f);
		}
#line 1630 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 250 "parser.y" /* yacc.c:1646  */
    {
			//return type mismatch with declaration
			SymbolInfo * sp = symbolTable -> Lookup((yyvsp[-3].si)->getName());
			if(sp!=NULL)
			{
				string type = (yyvsp[-4].si) -> getType();
				string f_name = (yyvsp[-3].si) -> getName();
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
				symbolTable -> Insert((yyvsp[-3].si)->getName(), "ID", (yyvsp[-4].si)->getType(), "function", f);
			}
		}
#line 1675 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 291 "parser.y" /* yacc.c:1646  */
    {
		   	params.clear();
			string name = (yyvsp[-6].si)->getName()+" "+(yyvsp[-5].si)->getName()+"("+(yyvsp[-3].si)->getName()+")"+(yyvsp[0].si)->getName();
			string type = "type_specifier ID LPAREN parameter_list RPAREN compound_statement";
			(yyval.si) = assignProduction(name, type, "func_definition", lg);
		   }
#line 1686 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 302 "parser.y" /* yacc.c:1646  */
    { 
			SymbolInfo * sp = symbolTable -> Lookup((yyvsp[-2].si)->getName());

			if(sp!=NULL)
			{
				string type = (yyvsp[-3].si) -> getType();
				string f_name = (yyvsp[-2].si) -> getName();
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
				symbolTable -> Insert((yyvsp[-2].si)->getName(), "ID", (yyvsp[-3].si)->getType(), "function"); 
			}
			params.clear();
			
		}
#line 1713 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 325 "parser.y" /* yacc.c:1646  */
    {
		  	params.clear();
			string name = (yyvsp[-5].si)->getName()+" "+(yyvsp[-4].si)->getName()+"()"+(yyvsp[0].si)->getName();
			string type = "type_specifier ID LPAREN RPAREN compound_statement";
			(yyval.si) = assignProduction(name, type, "func_definition", lg);
			
			if(!(yyvsp[-4].si)->getName().compare("main"))
			{
				(yyval.si)->code += "main proc\nmov ax,@data\nmov ds,ax\n";
				(yyval.si)->code += (yyvsp[0].si) -> code;
				(yyval.si)->code += "exit:\nmov ah,4ch\nint 21h\nmain endp\nend main";
			}			
		  }
#line 1731 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 342 "parser.y" /* yacc.c:1646  */
    {
			
			if(FindParam((yyvsp[0].si)->getName())) {
    				printError("Multiple declaration of a in parameter");
			}
			
			string type = "parameter_list COMMA type_specifier ID";
			(yyval.si) = assignProduction((yyvsp[-3].si)->getName()+","+(yyvsp[-1].si)->getName()+" "+(yyvsp[0].si)->getName(), type, "parameter_list", lg);
			//symbolTable -> Insert($4->getName(), "ID", $3->getType());
			
			SymbolInfo * s = new SymbolInfo((yyvsp[0].si)->getName(), "ID");
			s -> setDataType((yyvsp[-1].si)->getType());
			params.push_back(s);
			
		}
#line 1751 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 358 "parser.y" /* yacc.c:1646  */
    {
			string type = "parameter_list COMMA type_specifier";
			(yyval.si) = assignProduction((yyvsp[-2].si)->getName()+","+(yyvsp[0].si)->getName(), type, "parameter_list", lg);
		}
#line 1760 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 363 "parser.y" /* yacc.c:1646  */
    {
			(yyval.si) = assignProduction((yyvsp[-1].si)->getName()+" "+(yyvsp[0].si)->getName(), "type_specifier ID", "parameter_list", lg);
			//symbolTable -> Insert($2->getName(), "ID", $1->getType());
			
			SymbolInfo * s = new SymbolInfo((yyvsp[0].si)->getName(), "ID");
			s -> setDataType((yyvsp[-1].si)->getType());
			params.push_back(s);
		}
#line 1773 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 372 "parser.y" /* yacc.c:1646  */
    {
			(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "type_specifier", "parameter_list", lg);
		}
#line 1781 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 378 "parser.y" /* yacc.c:1646  */
    {symbolTable -> EnterScope(); insertParams(); }
#line 1787 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 379 "parser.y" /* yacc.c:1646  */
    {
 		    	(yyval.si) = assignProduction("{\n"+(yyvsp[-1].si)->getName()+"}\n", "LCURL statements RCURL", "compound_statement", lg);
 		    	symbolTable -> PrintInFile(lg);
 		    	symbolTable -> ExitScope();
 		    	(yyval.si)->code += (yyvsp[-1].si)->code;
 		    }
#line 1798 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 385 "parser.y" /* yacc.c:1646  */
    {symbolTable -> EnterScope(); insertParams(); }
#line 1804 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 386 "parser.y" /* yacc.c:1646  */
    {
 		    	(yyval.si) = assignProduction("{\n}\n", "LCURL RCURL", "compound_statement", lg);
 		    	symbolTable -> PrintInFile(lg);
 		    	symbolTable -> ExitScope();
 		    	(yyval.si)->code = "";
 		    }
#line 1815 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 395 "parser.y" /* yacc.c:1646  */
    {
		     string type = "type_specifier declaration_list SEMICOLON";
		     string name = (yyvsp[-2].si)->getName()+" "+(yyvsp[-1].si)->getName()+";";
		     (yyval.si) = new SymbolInfo(name, type);
		     
		     fprintf(lg, "Line %d: var_declaration : %s\n\n", line_count, type.c_str());
	 	     if(!(yyvsp[-2].si)->getType().compare("VOID"))
	 	     {
	 		printError("Variable type cannot be void");
	 	     }	
	 	     fprintf(lg, "%s\n\n", name.c_str());
		 }
#line 1832 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 409 "parser.y" /* yacc.c:1646  */
    {
				(yyval.si) = assignProduction("int", "INT", "type_specifier", lg);
				data_type = "INT";
			}
#line 1841 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 413 "parser.y" /* yacc.c:1646  */
    {
				(yyval.si) = assignProduction("float", "FLOAT", "type_specifier", lg);
				data_type = "FLOAT";
			}
#line 1850 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 417 "parser.y" /* yacc.c:1646  */
    {
				(yyval.si) = assignProduction("void", "VOID", "type_specifier", lg);
				data_type = "VOID";
			}
#line 1859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 424 "parser.y" /* yacc.c:1646  */
    {
 		  	string type = "declaration_list COMMA ID";
	 		if(symbolTable -> Insert((yyvsp[0].si)->getName(), "ID", data_type, "single_var"))
		 		(yyval.si) = assignProduction((yyvsp[-2].si)->getName()+","+(yyvsp[0].si)->getName(), type, "declaration_list", lg);
		 	else{
		 		printError("Multiple declaration of "+(yyvsp[0].si)->getName());	
		 		(yyval.si) = assignProduction((yyvsp[-2].si)->getName()+","+(yyvsp[0].si)->getName(), type, "declaration_list", lg);
		 	}	
		 		 		
	 		SymbolInfo * temp = new SymbolInfo((yyvsp[0].si)->getName()+symbolTable->getID(), "notarray");
	 		asm_vars.push_back(temp);
 		  }
#line 1876 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 437 "parser.y" /* yacc.c:1646  */
    {
 		  	string type = "declaration_list COMMA ID LTHIRD CONST_INT RTHIRD";
 		  	string name = (yyvsp[-5].si)->getName()+","+(yyvsp[-3].si)->getName()+"["+(yyvsp[-1].si)->getName()+"]";
 		  	if(symbolTable -> Insert((yyvsp[-3].si)->getName(), "ID", data_type, "array"))
	 			(yyval.si) = assignProduction(name, type, "declaration_list", lg);
	 		else{
	 			printError("Multiple declaration of "+(yyvsp[-3].si)->getName());
	 			(yyval.si) = assignProduction(name, type, "declaration_list", lg);
	 		}
	 		
	 		//array	 		
	 		SymbolInfo * temp = new SymbolInfo((yyvsp[-3].si)->getName()+symbolTable->getID(), (yyvsp[-1].si)->getName());
	 		asm_vars.push_back(temp);		
	 		
 		  }
#line 1896 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 453 "parser.y" /* yacc.c:1646  */
    {
 		  	if(symbolTable -> Insert((yyvsp[0].si)->getName(), "ID", data_type, "single_var"))
	 			(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "ID", "declaration_list", lg);
	 		else{
	 			printError("Multiple declaration of "+(yyvsp[0].si)->getName());
	 			(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "ID", "declaration_list", lg);
	 		}
	 		
	 		SymbolInfo * temp = new SymbolInfo((yyvsp[0].si)->getName()+symbolTable->getID(), "notarray");
	 		asm_vars.push_back(temp);
 		  }
#line 1912 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 465 "parser.y" /* yacc.c:1646  */
    {
 		  	string type = "ID LTHIRD CONST_INT RTHIRD";
 		  	if(symbolTable -> Insert((yyvsp[-3].si)->getName(), "ID", data_type,"array"))
		 		(yyval.si) = assignProduction((yyvsp[-3].si)->getName()+"["+(yyvsp[-1].si)->getName()+"]", type, "declaration_list", lg);
		 	else{
		 		printError("Multiple declaration of "+(yyvsp[-3].si)->getName());
		 		(yyval.si) = assignProduction((yyvsp[-3].si)->getName()+"["+(yyvsp[-1].si)->getName()+"]", type, "declaration_list", lg);
		 	}
		 	
		 	//array	 		
	 		SymbolInfo * temp = new SymbolInfo((yyvsp[-3].si)->getName()+symbolTable->getID(), (yyvsp[-1].si)->getName());
	 		asm_vars.push_back(temp);	
 		  }
#line 1930 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 481 "parser.y" /* yacc.c:1646  */
    {
	   	(yyval.si) = assignProduction((yyvsp[0].si)->getName()+"\n", "statement", "statements", lg);
	   	(yyval.si)->code += (yyvsp[0].si)->code;
	   	
	   }
#line 1940 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 487 "parser.y" /* yacc.c:1646  */
    {
	   	(yyval.si) = assignProduction((yyvsp[-1].si)->getName()+(yyvsp[0].si)->getName()+"\n", "statements statement", "statements", lg);
	    	(yyval.si)->code += (yyvsp[-1].si)->code+(yyvsp[0].si)->code;
	   }
#line 1949 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 494 "parser.y" /* yacc.c:1646  */
    {
	 	(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "var_declaration", "statement", lg);
	 	(yyval.si)->code = "";
	  }
#line 1958 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 499 "parser.y" /* yacc.c:1646  */
    {
	 	(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "expression_statement", "statement", lg);
	 	(yyval.si)->code += (yyvsp[0].si)->code;
	 	//cout << $$->code;
	  }
#line 1968 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 505 "parser.y" /* yacc.c:1646  */
    {
	 	(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "compound_statement", "statement", lg);
	 	(yyval.si)->code += (yyvsp[0].si)->code;
	  }
#line 1977 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 510 "parser.y" /* yacc.c:1646  */
    {
	  	string type = "FOR LPAREN expression_statement expression_statement expression RPAREN statement";
	  	string name = "for ("+(yyvsp[-4].si)->getName()+(yyvsp[-3].si)->getName()+(yyvsp[-2].si)->getName()+") "+(yyvsp[0].si)->getName();
	 	(yyval.si) = assignProduction(name, type, "statement", lg);
		
		(yyval.si)->code += (yyvsp[-4].si)->code; 
		char * for_loop = newLabel();
		char * exit_label = newLabel();
		(yyval.si)->code += string(for_loop) + ":\n";
		(yyval.si)->code += (yyvsp[-3].si)->code;
		(yyval.si)->code += "mov ax, " + (yyvsp[-3].si)->symbol + "\n";
		(yyval.si)->code += "cmp ax, 0\n";
		(yyval.si)->code += "je " + string(exit_label) + "\n";
		(yyval.si)->code += (yyvsp[-2].si)->code;
		(yyval.si)->code += (yyvsp[0].si)->code;
		(yyval.si)->code += "jmp " + string(for_loop) + "\n";
		(yyval.si)->code += string(exit_label)+":\n";
				
	  }
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 530 "parser.y" /* yacc.c:1646  */
    {
	  	string type = "IF LPAREN expression RPAREN statement";
	 	(yyval.si) = assignProduction("if ("+(yyvsp[-2].si)->getName()+") "+(yyvsp[0].si)->getName(), type, "statement", lg);

		(yyval.si)->code += (yyvsp[-2].si)->code;		
		char *label=newLabel();
		(yyval.si)->code+="mov ax, "+(yyvsp[-2].si)->symbol+"\n";
		(yyval.si)->code+="cmp ax, 0\n";
		(yyval.si)->code+="je "+string(label)+"\n";
		(yyval.si)->code+=(yyvsp[0].si)->code;
		(yyval.si)->code+=string(label)+":\n";	 	
	  }
#line 2018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 543 "parser.y" /* yacc.c:1646  */
    {
	  	string type = "IF LPAREN expression RPAREN statement ELSE statement";
	 	(yyval.si) = assignProduction("if ("+(yyvsp[-4].si)->getName()+") "+(yyvsp[-2].si)->getName()+"else "+(yyvsp[0].si)->getName(), type, "statement", lg);
	 	
		(yyval.si)->code += (yyvsp[-4].si)->code;		
		char *label1=newLabel();
		char *label2 = newLabel();
		(yyval.si)->code+="mov ax, "+(yyvsp[-4].si)->symbol+"\n";
		(yyval.si)->code+="cmp ax, 0\n";
		(yyval.si)->code+="je "+string(label1)+"\n";
		(yyval.si)->code+=(yyvsp[-2].si)->code;
		(yyval.si)->code+= "jmp " + string(label2) + "\n";
		(yyval.si)->code+=string(label1)+":\n";
		(yyval.si)->code+=(yyvsp[0].si)->code;
		(yyval.si)->code+=string(label2) + ":\n"; 		 	
	  }
#line 2039 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 560 "parser.y" /* yacc.c:1646  */
    {
	  	string type = "WHILE LPAREN expression RPAREN statement";
	 	(yyval.si) = assignProduction("while ("+(yyvsp[-2].si)->getName()+") "+(yyvsp[0].si)->getName(), type, "statement", lg);
	 	
	 	char * loop_label = newLabel();
	 	char * exit_label = newLabel();
	 	(yyval.si)->code += string(loop_label) + ":\n";
	 	(yyval.si)->code += (yyvsp[-2].si)->code;
	 	(yyval.si)->code += "mov ax, " + (yyvsp[-2].si)->symbol + "\n";
	 	(yyval.si)->code += "cmp ax, 0\n";
	 	(yyval.si)->code += "je " + string(exit_label) + "\n";
	 	(yyval.si)->code += (yyvsp[0].si)->code;
	 	(yyval.si)->code += "jmp " + string(loop_label)+"\n";
	 	(yyval.si)->code += string(exit_label)+":\n";
	  }
#line 2059 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 576 "parser.y" /* yacc.c:1646  */
    {
	  	string type = "PRINTLN LPAREN ID RPAREN SEMICOLON";
	  	string name = "printf("+(yyvsp[-2].si)->getName()+");";
	  	SymbolInfo * sp = symbolTable -> Lookup((yyvsp[-2].si)->getName());
	 	(yyval.si) = new SymbolInfo(name, type);
	 	
	 	fprintf(lg, "Line %d: variable : %s\n\n", line_count, type.c_str());
	 	if(sp==NULL)
	 	{
	 		printError("Undeclared variable "+(yyvsp[-2].si)->getName());
	 	}
	 	fprintf(lg, "%s\n\n", name.c_str());
	 	
	 	string code_ = "mov ax, " + (yyvsp[-2].si)->getName()+symbolTable->getID() + "\n";
	 	code_ += "mov print_var, ax\n";
	 	code_ += "call print\n";
	 	(yyval.si)->code += code_;
	 	
	  }
#line 2083 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 596 "parser.y" /* yacc.c:1646  */
    {
	  	string type = "RETURN expression SEMICOLON";
	 	(yyval.si) = assignProduction("return "+(yyvsp[-1].si)->getName()+";", type, "statement", lg);
	  }
#line 2092 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 603 "parser.y" /* yacc.c:1646  */
    {
				(yyval.si) = assignProduction(";", "SEMICOLON", "expression_statement", lg);
			}
#line 2100 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 607 "parser.y" /* yacc.c:1646  */
    {
				(yyval.si) = assignProduction((yyvsp[-1].si)->getName()+";", "expression SEMICOLON", "expression_statement", lg);
				(yyval.si) -> code += (yyvsp[-1].si)->code;
				(yyval.si)-> symbol = (yyvsp[-1].si)-> symbol;
			}
#line 2110 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 615 "parser.y" /* yacc.c:1646  */
    {
 	 	SymbolInfo * sp = symbolTable -> Lookup((yyvsp[0].si)->getName());
 	 	string var_id = symbolTable -> Lookup_ID((yyvsp[0].si)->getName());
	 	
	 	string name = (yyvsp[0].si)->getName();
	 	string type = "ID";
	 	(yyval.si) = new SymbolInfo(name, type);
	 	if(sp != NULL) (yyval.si) -> setDataType(sp -> getDataType());
	 	
	 	fprintf(lg, "Line %d: variable : %s\n\n", line_count, type.c_str());
	 	if(sp!=NULL){
	 		if(!sp->getVarType().compare("array")) printError("Type mismatch, " + (yyvsp[0].si)->getName() + " is an array");
	 	}	
	 	else printError("Undeclared variable "+(yyvsp[0].si)->getName());
	 	fprintf(lg, "%s\n\n", name.c_str());
	 	
	 	(yyval.si)->symbol = (yyvsp[0].si)->getName()+var_id;
	 	(yyval.si)->setVarType("notarray");
	 	(yyval.si)->code="";
	 	
	 }
#line 2136 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 637 "parser.y" /* yacc.c:1646  */
    {
	 	string type = "ID LTHIRD expression RTHIRD";
	 	string name = (yyvsp[-3].si)->getName()+"["+(yyvsp[-1].si)->getName()+"]";
	 	
	 	(yyval.si) = new SymbolInfo(name, type);
	 	SymbolInfo * sp = symbolTable -> Lookup((yyvsp[-3].si)->getName());
 	 	string var_id = symbolTable -> Lookup_ID((yyvsp[-3].si)->getName());
 	 	
	 	if(sp != NULL) (yyval.si) -> setDataType(sp -> getDataType());
	 	
	 	fprintf(lg, "Line %d: variable : %s\n\n", line_count, type.c_str());
	 	if(sp == NULL) printError("Undeclared variable "+(yyvsp[-3].si)->getName());
	 	if(sp!=NULL)
	 		if(sp->getVarType().compare("array")) printError((yyvsp[-3].si)->getName() + " not an array");
	 	if((yyvsp[-1].si)->getDataType().compare("INT")) printError("Expression inside third brackets not an integer");
	 	fprintf(lg, "%s\n\n", name.c_str());
	 	
	 	(yyval.si)->symbol = (yyvsp[-3].si)->getName()+var_id+"[bx]";
	 	(yyval.si)->code=(yyvsp[-1].si)->code+"mov bx, " +(yyvsp[-1].si)->symbol +"\nadd bx, bx\n";
	  	(yyval.si)->setVarType("array");
	 }
#line 2162 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 661 "parser.y" /* yacc.c:1646  */
    {
 	   	string name = (yyvsp[0].si)->getName();
 	   	string type = "logic expression";
 	   	(yyval.si) = new SymbolInfo(name, type);
 	   	
 	   	fprintf(lg, "Line %d: expression : %s\n\n", line_count, type.c_str());
 	   	if(!(yyvsp[0].si)->getDataType().compare("VOID")) printError("Expression contains type : void");
 	   	fprintf(lg, "%s\n\n", name.c_str());
 	   	
 	   	(yyval.si) -> setDataType((yyvsp[0].si)->getDataType());
 	   	
 	   	(yyval.si)->code += (yyvsp[0].si)->code;
 	   	(yyval.si)->symbol = (yyvsp[0].si)->symbol;
 	   	
 	   	
 	   }
#line 2183 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 678 "parser.y" /* yacc.c:1646  */
    {
	   	string type = "variable ASSIGNOP logic_expression";
	   	string name = (yyvsp[-2].si)->getName()+"="+(yyvsp[0].si)->getName();
	   	(yyval.si) = new SymbolInfo(name, type);
	   	
	   	//look-up var in table
	   	string var_name = "";
	   	if ((yyvsp[-2].si)->getName().find("[") != string::npos) {
        		 var_name = (yyvsp[-2].si)->getName().substr(0, (yyvsp[-2].si)->getName().find("["));
        	}else var_name = (yyvsp[-2].si)->getName();
        	SymbolInfo * sp = symbolTable -> Lookup(var_name);
	   	
	   	//print errors
	   	fprintf(lg, "Line %d: expression : %s\n\n", line_count, type.c_str());
	   	if(sp!=NULL)
	   	{
	   		string temp = "";
	   		if(!(yyvsp[0].si)->getDataType().compare("VOID")) printError("Void function used in expression");
	   		else
	   		{
		   		if(!(yyvsp[0].si)->getDataType().compare("NULL"));
		   		else if(!sp->getDataType().compare("FLOAT") && !(yyvsp[0].si)->getDataType().compare("INT"));
		   		else if(sp->getDataType().compare((yyvsp[0].si)->getDataType())) printError("Type Mismatch");
	   		}	   		
	   		(yyval.si) -> setDataType(sp->getDataType());
	   	}else (yyval.si) -> setDataType("NULL");
	   	fprintf(lg, "%s\n\n", name.c_str());
	   	
	   	(yyval.si)->code += (yyvsp[0].si)->code;
		(yyval.si)->code +="mov ax, "+(yyvsp[0].si)->symbol+"\n";
		(yyval.si)->code += (yyvsp[-2].si)->code;
		(yyval.si)->code+= "mov  "+(yyvsp[-2].si)->symbol+", ax\n";	   	
	   }
#line 2221 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 714 "parser.y" /* yacc.c:1646  */
    {
		 	(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "rel_expression", "logic_expression", lg);
		 	(yyval.si) -> setDataType((yyvsp[0].si)->getDataType());
		 	
		  	(yyval.si) -> code += (yyvsp[0].si) -> code;
		  	(yyval.si) -> symbol = (yyvsp[0].si) -> symbol;			 	
		 }
#line 2233 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 722 "parser.y" /* yacc.c:1646  */
    {
		 	string type = "rel_expression LOGICOP rel_expression";
			(yyval.si) = assignProduction((yyvsp[-2].si)->getName()+(yyvsp[-1].si)->getName()+(yyvsp[0].si)->getName(), type , "logic_expression", lg);
			
			if(!(yyvsp[-2].si)->getDataType().compare("VOID") || !(yyvsp[0].si)->getDataType().compare("VOID")) 
				(yyval.si) -> setDataType("VOID");
			else (yyval.si) -> setDataType("INT");
			
			(yyval.si)->code += (yyvsp[-2].si)->code+(yyvsp[0].si)->code;
			char * temp = newTemp();
			(yyval.si)->symbol = string(temp);
			
			(yyval.si)->code += "mov ax, " + (yyvsp[0].si)->symbol + "\n";
			(yyval.si)->code += "mov bx, " + (yyvsp[-2].si)->symbol +"\n";
			char * label1 = newLabel();
			char * label2 = newLabel();
			
			if(!(yyvsp[-1].si)->getName().compare("&&"))
			{
				//see if anyone is zero, jump to label1 if so
				(yyval.si)->code += "cmp ax, 0\nje "+string(label1)+"\n";
				(yyval.si)->code += "cmp bx, 0\nje "+string(label1)+"\n";

				//both non zero, set 1 and jump to label2...skipping label1
				(yyval.si)->code += "mov ax, 1\n";
				(yyval.si)->code += "mov " + string(temp) + ", ax\n";
				(yyval.si)->code += "jmp " + string(label2) + "\n";
				
				//label1: set temp var = 0
				(yyval.si)->code += string(label1) + ":\n";
				(yyval.si)->code += "mov ax, 0\n";
				(yyval.si)->code += "mov " + string(temp) + ", ax\n";
				
				(yyval.si)->code += string(label2) + ":\n";
			}
			else //OR
			{
				//see if anyone is 1, jump to label1 if so
				(yyval.si)->code += "cmp ax, 1\nje "+string(label1)+"\n";
				(yyval.si)->code += "cmp bx, 1\nje "+string(label1)+"\n";

				//both zero, set 0 and jump to label2...skipping label1
				(yyval.si)->code += "mov ax, 0\n";
				(yyval.si)->code += "mov " + string(temp) + ", ax\n";
				(yyval.si)->code += "jmp " + string(label2) + "\n";
				
				//label1: set temp var = 1
				(yyval.si)->code += string(label1) + ":\n";
				(yyval.si)->code += "mov ax, 1\n";
				(yyval.si)->code += "mov " + string(temp) + ", ax\n";
				
				(yyval.si)->code += string(label2) + ":\n";				
			}
		 }
#line 2292 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 779 "parser.y" /* yacc.c:1646  */
    {
			(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "simple_expression", "rel_expression", lg);
			(yyval.si) -> setDataType((yyvsp[0].si)->getDataType());
		  	
		  	(yyval.si) -> code += (yyvsp[0].si) -> code;
		  	(yyval.si) -> symbol = (yyvsp[0].si) -> symbol;	
		  	//cout << $1->code;		
		}
#line 2305 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 788 "parser.y" /* yacc.c:1646  */
    {
			string type = "simple_expression RELOP simple_expression";
			(yyval.si) = assignProduction((yyvsp[-2].si)->getName()+(yyvsp[-1].si)->getName()+(yyvsp[0].si)->getName(), type , "rel_expression", lg);
			
			if(!(yyvsp[-2].si)->getDataType().compare("VOID") || !(yyvsp[0].si)->getDataType().compare("VOID")) 
				(yyval.si) -> setDataType("VOID");
			else (yyval.si) -> setDataType("INT");
	
			(yyval.si)->code += (yyvsp[-2].si)->code+(yyvsp[0].si)->code;
			(yyval.si)->code+="mov ax, " + (yyvsp[-2].si)->symbol+"\n";
			(yyval.si)->code+="cmp ax, " + (yyvsp[0].si)->symbol+"\n";
			char *temp=newTemp();
			char *label1=newLabel();
			char *label2=newLabel();
			
			if(!(yyvsp[-1].si)->getName().compare("<")){
				(yyval.si)->code+="jl " + string(label1)+"\n";
			}
			else if(!(yyvsp[-1].si)->getName().compare("<=")){
				(yyval.si)->code+="jle " + string(label1)+"\n";
			}
			else if(!(yyvsp[-1].si)->getName().compare(">")){
				(yyval.si)->code+="jg " + string(label1)+"\n";			
			}
			else if(!(yyvsp[-1].si)->getName().compare(">=")){
				(yyval.si)->code+="jge " + string(label1)+"\n";			
			}
			else if(!(yyvsp[-1].si)->getName().compare("==")){
				(yyval.si)->code+="je " + string(label1)+"\n";			
			}
			else{
				(yyval.si)->code+="jne " + string(label1)+"\n";			
			}
			
			(yyval.si)->code+="mov "+string(temp) +", 0\n";
			(yyval.si)->code+="jmp "+string(label2) +"\n";
			(yyval.si)->code+=string(label1)+":\nmov "+string(temp)+", 1\n";
			(yyval.si)->code+=string(label2)+":\n";
			(yyval.si)->symbol = string(temp);			
		}
#line 2350 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 831 "parser.y" /* yacc.c:1646  */
    {
		  	(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "term", "simple_expression", lg);
		  	(yyval.si) -> setDataType((yyvsp[0].si)->getDataType());
		  	
		  	(yyval.si) -> code += (yyvsp[0].si) -> code;
		  	(yyval.si) -> symbol = (yyvsp[0].si) -> symbol;
		  }
#line 2362 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 839 "parser.y" /* yacc.c:1646  */
    {
		  	string type = "simple_expression ADDOP term";
		  	(yyval.si) = assignProduction((yyvsp[-2].si)->getName()+(yyvsp[-1].si)->getName()+(yyvsp[0].si)->getName(), type , "simple_expression", lg);
		  	
		  	if(!(yyvsp[-2].si)->getDataType().compare("VOID") || !(yyvsp[0].si)->getDataType().compare("VOID")) 
				(yyval.si) -> setDataType("VOID");
		  	else if(!(yyvsp[-2].si)->getDataType().compare("FLOAT") || !(yyvsp[0].si)->getDataType().compare("FLOAT")) 
				(yyval.si) -> setDataType("FLOAT");
			else (yyval.si) -> setDataType("INT");
			
			(yyval.si)->code += (yyvsp[-2].si)->code+(yyvsp[0].si)->code;
			(yyval.si)->code += "mov ax, " + (yyvsp[-2].si)->symbol + "\n";
			if(!(yyvsp[-1].si)->getName().compare("+"))
				(yyval.si)->code += "add ax, " + (yyvsp[0].si)->symbol+ "\n";
			else 
				(yyval.si)->code += "sub ax, " + (yyvsp[0].si)->symbol+ "\n";
			char * temp = newTemp();
			(yyval.si) ->code += "mov " + string(temp)+", ax\n";
			
			(yyval.si) -> symbol = string(temp);
			//cout << $$->code;
		  }
#line 2389 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 864 "parser.y" /* yacc.c:1646  */
    {
     	(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "unary_expression", "term", lg);
     	(yyval.si) -> setDataType((yyvsp[0].si)->getDataType());
     	
     	(yyval.si)-> code +=  (yyvsp[0].si)->code;
     	(yyval.si)->symbol = (yyvsp[0].si)->symbol;
     }
#line 2401 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 872 "parser.y" /* yacc.c:1646  */
    {
     	string name = (yyvsp[-2].si)->getName()+(yyvsp[-1].si)->getName()+(yyvsp[0].si)->getName();
     	string type = "term MULOP unary_expression";
     	(yyval.si) = new SymbolInfo(name, type);
     	
     	fprintf(lg, "Line %d: term : %s\n\n", line_count, type.c_str());
     	
	if(!(yyvsp[-1].si)->getName().compare("%")){
		if((yyvsp[-2].si)->getDataType().compare("INT") || (yyvsp[0].si)->getDataType().compare("INT")) printError("Non-Integer operand on modulus operator");
		else if(!(yyvsp[0].si)->getName().compare("0"))
		{
			printError("Modulus by Zero");
		}
	}
	
	if(!(yyvsp[-2].si)->getDataType().compare("VOID") || !(yyvsp[0].si)->getDataType().compare("VOID")) 
		(yyval.si) -> setDataType("VOID");
	else if(!(yyvsp[-2].si)->getDataType().compare("FLOAT") || !(yyvsp[0].si)->getDataType().compare("FLOAT")) 
		(yyval.si) -> setDataType("FLOAT");
	else (yyval.si) -> setDataType("INT");		
	
	fprintf(lg, "%s\n\n", name.c_str());
	
	(yyval.si)->code += (yyvsp[-2].si)->code+(yyvsp[0].si)->code;
	(yyval.si)->code += "mov ax, "+ (yyvsp[-2].si)->symbol+"\n";
	(yyval.si)->code += "mov bx, "+ (yyvsp[0].si)->symbol+"\n";
	char *temp=newTemp();
	if(!(yyvsp[-1].si)->getName().compare("*")){
		(yyval.si)->code += "mul bx\n";
		(yyval.si)->code += "mov "+ string(temp) + ", ax\n";
	}
	else if(!(yyvsp[-1].si)->getName().compare("/")){
		// clear dx, perform 'div bx' and mov ax to temp
		(yyval.si)->code += "xor dx, dx\n";
		(yyval.si)->code += "div bx\n";
		(yyval.si)->code += "mov "+ string(temp) + ", ax\n";
	}
	else if(!(yyvsp[-1].si)->getName().compare("%")){
		// clear dx, perform 'div bx' and mov dx to temp
		(yyval.si)->code += "xor dx, dx\n";
		(yyval.si)->code += "div bx\n";
		(yyval.si)->code += "mov "+ string(temp) + ", dx\n";		
	}
	(yyval.si)->symbol = string(temp);
	//cout << $$->code ;
     }
#line 2452 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 921 "parser.y" /* yacc.c:1646  */
    {
		 	(yyval.si) = assignProduction((yyvsp[-1].si)->getName()+(yyvsp[0].si)->getName(), "ADDOP unary_expression", "unary_expression", lg);	
		 	if(!(yyvsp[0].si)->getDataType().compare("FLOAT")) 
		 	{
		 		(yyval.si) -> setDataType("FLOAT");	
		 	}else if(!(yyvsp[0].si)->getDataType().compare("VOID")) 
		 		(yyval.si) -> setDataType("VOID");	
		 	else (yyval.si) -> setDataType("INT");	
		 	
		 	(yyval.si)->code += (yyvsp[0].si)->code;
		 	(yyval.si)->symbol = (yyvsp[0].si)->symbol;
		 	
		 	//negate if -ve
		 	if(!(yyvsp[-1].si)->getName().compare("-"))
		 	{
			 	char *temp=newTemp();
				(yyval.si)->code="mov ax, " + (yyvsp[0].si)->symbol + "\n";
				(yyval.si)->code+="neg ax\n";
				(yyval.si)->code+="mov "+string(temp)+", ax";
				(yyval.si)->symbol = string(temp);		 		
			}
			
		 }
#line 2480 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 945 "parser.y" /* yacc.c:1646  */
    {
		 	(yyval.si) = assignProduction("!"+(yyvsp[0].si)->getName(), "NOT unary_expression", "unary_expression", lg);
		 	if(!(yyvsp[0].si)->getDataType().compare("VOID"))
		 		 (yyval.si) -> setDataType("VOID");
		 	else (yyval.si) -> setDataType("INT");	
		 	
		 	(yyval.si) -> code += (yyvsp[0].si) -> code;
		 	char *temp=newTemp();
			(yyval.si)->code="mov ax, " + (yyvsp[0].si)->symbol + "\n";
			(yyval.si)->code+="not ax\n";
			(yyval.si)->code+="mov "+string(temp)+", ax";		
	
			(yyval.si)->symbol = string(temp); 	
		 		
		 }
#line 2500 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 961 "parser.y" /* yacc.c:1646  */
    {
		 	(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "factor", "unary_expression", lg);
			(yyval.si) -> setDataType((yyvsp[0].si)->getDataType());	
			(yyval.si) -> code += (yyvsp[0].si)->code;
			(yyval.si) -> symbol = (yyvsp[0].si) -> symbol;			
		 }
#line 2511 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 970 "parser.y" /* yacc.c:1646  */
    {
		(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "variable", "factor", lg);
		(yyval.si) -> setDataType((yyvsp[0].si)->getDataType());	
		(yyval.si)->symbol = (yyvsp[0].si)->symbol;	
		(yyval.si)->code += (yyvsp[0].si)->code;
	}
#line 2522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 977 "parser.y" /* yacc.c:1646  */
    {
		string name = (yyvsp[-3].si)->getName()+"("+(yyvsp[-1].si)->getName()+")";
		string type = "ID LPAREN argument_list RPAREN";
		(yyval.si) = new SymbolInfo(name, type);
		
		fprintf(lg, "Line %d: factor : %s\n\n", line_count, type.c_str());
		
		SymbolInfo * sp = symbolTable -> Lookup((yyvsp[-3].si)->getName());
		if(sp != NULL) {
			(yyval.si) -> setDataType(sp->getDataType());
			if(!sp->getVarType().compare("function"))
			{
				if(sp->getFunctionInfo().param_num() != args.size())
					printError("Total number of arguments mismatch in function "+(yyvsp[-3].si)->getName());
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
							printError(to_string(i)+"th argument mismatch in function "+(yyvsp[-3].si)->getName());
							break;
						}
					}
				}
			}
			else printError((yyvsp[-3].si)->getName()+" is not a function");	
			
		}
		else {
			printError("Undeclared function "+(yyvsp[-3].si)->getName());	
			(yyval.si) -> setDataType("NULL");
		}
		args.clear();
		fprintf(lg, "%s\n\n", name.c_str());
					
	}
#line 2568 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1019 "parser.y" /* yacc.c:1646  */
    {
		(yyval.si) = assignProduction("("+(yyvsp[-1].si)->getName()+")", "LPAREN expression RPAREN", "factor", lg);
		(yyval.si) -> setDataType((yyvsp[-1].si)->getDataType());
		(yyval.si)->symbol = (yyvsp[-1].si)->symbol;	
		(yyval.si)->code += (yyvsp[-1].si)->code;	
		//cout << $2->code;	
	}
#line 2580 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1027 "parser.y" /* yacc.c:1646  */
    {
		(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "CONST_INT", "factor", lg);	
		value_type = "INT";	
		(yyval.si) -> setDataType("INT");	
		(yyval.si)->symbol = (yyvsp[0].si)->getName();
		(yyval.si)->code = "";
	}
#line 2592 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1035 "parser.y" /* yacc.c:1646  */
    {
		(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "CONST_FLOAT", "factor", lg);
		value_type = "FLOAT";
		(yyval.si) -> setDataType("FLOAT");	
	}
#line 2602 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1041 "parser.y" /* yacc.c:1646  */
    {
		(yyval.si) = assignProduction((yyvsp[-1].si)->getName()+"++", "variable INCOP", "factor", lg);
		(yyval.si) -> setDataType((yyvsp[-1].si)->getDataType());	
		
		(yyval.si)->code += (yyvsp[-1].si)->code;
		char * temp = newTemp();
		(yyval.si)->code += "mov ax, "+(yyvsp[-1].si)->symbol+"\nmov "+ string(temp) + ", ax\ninc "+(yyvsp[-1].si)->symbol+"\n";
		(yyval.si)->symbol = string(temp);
		
	}
#line 2617 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1052 "parser.y" /* yacc.c:1646  */
    {
		(yyval.si) = assignProduction((yyvsp[-1].si)->getName()+"--", "variable DECOP", "factor", lg);
		(yyval.si) -> setDataType((yyvsp[-1].si)->getDataType());
		
		(yyval.si)->code+= (yyvsp[-1].si)->code;
		char *temp = newTemp();
		(yyval.si)->code += "mov ax, "+(yyvsp[-1].si)->symbol+"\nmov "+ string(temp) + ", ax\ndec "+(yyvsp[-1].si)->symbol+"\n";
		(yyval.si)->symbol = string(temp);
	}
#line 2631 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1064 "parser.y" /* yacc.c:1646  */
    {
			(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "arguments", "argument_list", lg);
		}
#line 2639 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1068 "parser.y" /* yacc.c:1646  */
    {
			(yyval.si) = assignProduction("", "", "argument_list", lg);
		}
#line 2647 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1074 "parser.y" /* yacc.c:1646  */
    {
			(yyval.si) = assignProduction((yyvsp[-2].si)->getName()+","+(yyvsp[0].si)->getName(), "arguments COMMA logic_expression", "arguments", lg);
			args.push_back((yyvsp[0].si)->getDataType());
			
		}
#line 2657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1080 "parser.y" /* yacc.c:1646  */
    {
	      		(yyval.si) = assignProduction((yyvsp[0].si)->getName(), "logic_expression", "arguments", lg);
			args.push_back((yyvsp[0].si)->getDataType());
	      }
#line 2666 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2670 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1087 "parser.y" /* yacc.c:1906  */

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

