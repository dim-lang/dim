/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     FT_EOF = 258,
     FT_TRUE = 259,
     FT_FALSE = 260,
     FT_LET = 261,
     FT_NIL = 262,
     FT_IF = 263,
     FT_ELSEIF = 264,
     FT_ELSE = 265,
     FT_FOR = 266,
     FT_WHILE = 267,
     FT_BREAK = 268,
     FT_CONTINUE = 269,
     FT_SWITCH = 270,
     FT_CASE = 271,
     FT_OTHERWISE = 272,
     FT_FUNC = 273,
     FT_CLASS = 274,
     FT_TYPE = 275,
     FT_ISINSTANCE = 276,
     FT_IMPORT = 277,
     FT_RETURN = 278,
     FT_VOID = 279,
     FT_LOGICALAND = 280,
     FT_LOGICALOR = 281,
     FT_LOGICALNOT = 282,
     FT_INTEGER_KEYWORD = 283,
     FT_UNSIGNED_INTEGER_KEYWORD = 284,
     FT_DOUBLE_KEYWORD = 285,
     FT_IDENTIFIER = 286,
     FT_INTEGER = 287,
     FT_DOUBLE = 288,
     FT_STRING = 289,
     FT_ADD = 290,
     FT_SUB = 291,
     FT_MUL = 292,
     FT_DIV = 293,
     FT_MOD = 294,
     FT_BITNOT = 295,
     FT_BITAND = 296,
     FT_BITOR = 297,
     FT_BITCOMPLEMENT = 298,
     FT_BITXOR = 299,
     FT_ASSIGN = 300,
     FT_ADDASSIGN = 301,
     FT_SUBASSIGN = 302,
     FT_MULASSIGN = 303,
     FT_DIVASSIGN = 304,
     FT_MODASSIGN = 305,
     FT_EQ = 306,
     FT_NEQ = 307,
     FT_LT = 308,
     FT_LE = 309,
     FT_GT = 310,
     FT_GE = 311,
     FT_LPAREN = 312,
     FT_RPAREN = 313,
     FT_LBRACKET = 314,
     FT_RBRACKET = 315,
     FT_LBRACE = 316,
     FT_RBRACE = 317,
     FT_COMMA = 318,
     FT_SEMI = 319,
     FT_QUESTION = 320,
     FT_COLON = 321,
     FT_DOT = 322
   };
#endif
/* Tokens.  */
#define FT_EOF 258
#define FT_TRUE 259
#define FT_FALSE 260
#define FT_LET 261
#define FT_NIL 262
#define FT_IF 263
#define FT_ELSEIF 264
#define FT_ELSE 265
#define FT_FOR 266
#define FT_WHILE 267
#define FT_BREAK 268
#define FT_CONTINUE 269
#define FT_SWITCH 270
#define FT_CASE 271
#define FT_OTHERWISE 272
#define FT_FUNC 273
#define FT_CLASS 274
#define FT_TYPE 275
#define FT_ISINSTANCE 276
#define FT_IMPORT 277
#define FT_RETURN 278
#define FT_VOID 279
#define FT_LOGICALAND 280
#define FT_LOGICALOR 281
#define FT_LOGICALNOT 282
#define FT_INTEGER_KEYWORD 283
#define FT_UNSIGNED_INTEGER_KEYWORD 284
#define FT_DOUBLE_KEYWORD 285
#define FT_IDENTIFIER 286
#define FT_INTEGER 287
#define FT_DOUBLE 288
#define FT_STRING 289
#define FT_ADD 290
#define FT_SUB 291
#define FT_MUL 292
#define FT_DIV 293
#define FT_MOD 294
#define FT_BITNOT 295
#define FT_BITAND 296
#define FT_BITOR 297
#define FT_BITCOMPLEMENT 298
#define FT_BITXOR 299
#define FT_ASSIGN 300
#define FT_ADDASSIGN 301
#define FT_SUBASSIGN 302
#define FT_MULASSIGN 303
#define FT_DIVASSIGN 304
#define FT_MODASSIGN 305
#define FT_EQ 306
#define FT_NEQ 307
#define FT_LT 308
#define FT_LE 309
#define FT_GT 310
#define FT_GE 311
#define FT_LPAREN 312
#define FT_RPAREN 313
#define FT_LBRACKET 314
#define FT_RBRACKET 315
#define FT_LBRACE 316
#define FT_RBRACE 317
#define FT_COMMA 318
#define FT_SEMI 319
#define FT_QUESTION 320
#define FT_COLON 321
#define FT_DOT 322




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 11 "bison.y"
{
    char *literal;
    int token;
}
/* Line 1529 of yacc.c.  */
#line 188 "bison.tab.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

