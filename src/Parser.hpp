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
     FT_FUNC = 270,
     FT_CLASS = 271,
     FT_TYPE = 272,
     FT_ISINSTANCE = 273,
     FT_IMPORT = 274,
     FT_RETURN = 275,
     FT_VOID = 276,
     FT_LOGICALAND = 277,
     FT_LOGICALOR = 278,
     FT_LOGICALNOT = 279,
     FT_IDENTIFIER = 280,
     FT_INTEGER = 281,
     FT_REAL = 282,
     FT_STRING = 283,
     FT_ADD = 284,
     FT_SUB = 285,
     FT_MUL = 286,
     FT_DIV = 287,
     FT_MOD = 288,
     FT_BITNOT = 289,
     FT_BITAND = 290,
     FT_BITOR = 291,
     FT_BITCOMPLEMENT = 292,
     FT_BITXOR = 293,
     FT_INC = 294,
     FT_DEC = 295,
     FT_ASSIGN = 296,
     FT_ADDASSIGN = 297,
     FT_SUBASSIGN = 298,
     FT_MULASSIGN = 299,
     FT_DIVASSIGN = 300,
     FT_MODASSIGN = 301,
     FT_EQ = 302,
     FT_NEQ = 303,
     FT_LT = 304,
     FT_LE = 305,
     FT_GT = 306,
     FT_GE = 307,
     FT_LPAREN = 308,
     FT_RPAREN = 309,
     FT_LBRACKET = 310,
     FT_RBRACKET = 311,
     FT_LBRACE = 312,
     FT_RBRACE = 313,
     FT_COMMA = 314,
     FT_SEMI = 315,
     FT_QUESTION = 316,
     FT_COLON = 317,
     FT_DOT = 318
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
#define FT_FUNC 270
#define FT_CLASS 271
#define FT_TYPE 272
#define FT_ISINSTANCE 273
#define FT_IMPORT 274
#define FT_RETURN 275
#define FT_VOID 276
#define FT_LOGICALAND 277
#define FT_LOGICALOR 278
#define FT_LOGICALNOT 279
#define FT_IDENTIFIER 280
#define FT_INTEGER 281
#define FT_REAL 282
#define FT_STRING 283
#define FT_ADD 284
#define FT_SUB 285
#define FT_MUL 286
#define FT_DIV 287
#define FT_MOD 288
#define FT_BITNOT 289
#define FT_BITAND 290
#define FT_BITOR 291
#define FT_BITCOMPLEMENT 292
#define FT_BITXOR 293
#define FT_INC 294
#define FT_DEC 295
#define FT_ASSIGN 296
#define FT_ADDASSIGN 297
#define FT_SUBASSIGN 298
#define FT_MULASSIGN 299
#define FT_DIVASSIGN 300
#define FT_MODASSIGN 301
#define FT_EQ 302
#define FT_NEQ 303
#define FT_LT 304
#define FT_LE 305
#define FT_GT 306
#define FT_GE 307
#define FT_LPAREN 308
#define FT_RPAREN 309
#define FT_LBRACKET 310
#define FT_RBRACKET 311
#define FT_LBRACE 312
#define FT_RBRACE 313
#define FT_COMMA 314
#define FT_SEMI 315
#define FT_QUESTION 316
#define FT_COLON 317
#define FT_DOT 318




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 9 "Parser.y"
{
    Node *node;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *ident;
    NVariableDeclaration *var_decl;
    std::vector<NVariableDeclaration*> *varvec;
    std::vector<NExpression*> *exprvec;

    std::string literal;
    int token;
}
/* Line 1529 of yacc.c.  */
#line 189 "Parser.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

