// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <string>

// eof
#define FT_EOF 0

// operators
#define FT_ADD 101           // +
#define FT_SUB 102           // -
#define FT_MUL 103           // *
#define FT_DIV 104           // /
#define FT_MOD 105           // %
#define FT_BITNOT 106        // !
#define FT_BITAND 107        // &
#define FT_BITOR 108         // |
#define FT_BITCOMPLEMENT 109 // ~
#define FT_BITXOR 110        // ^
#define FT_INC 111           // ++
#define FT_DEC 112           // --
#define FT_ASSIGN 113        // =
#define FT_ADDASSIGN 114     // +=
#define FT_SUBASSIGN 115     // -=
#define FT_MULASSIGN 116     // *=
#define FT_DIVASSIGN 117     // /=
#define FT_MODASSIGN 118     // %=

// comparator
#define FT_EQ 200  // ==
#define FT_NEQ 201 // !=
#define FT_LT 202  // <
#define FT_LE 203  // <=
#define FT_GT 204  // >
#define FT_GE 205  // >=

// punctuations
#define FT_LPAREN 301   // (
#define FT_RPAREN 302   // )
#define FT_LBRACKET 303 // [
#define FT_RBRACKET 304 // ]
#define FT_LBRACE 305   // {
#define FT_RBRACE 306   // }
#define FT_COMMA 307    // ,
#define FT_SEMI 308     // ;
#define FT_QUESTION 309 // ?
#define FT_COLON 310    // :
#define FT_DOT 311      // .

// keywords
#define FT_TRUE 401       // True
#define FT_FALSE 402      // False
#define FT_LET 403        // let
#define FT_NIL 404        // Nil
#define FT_IF 405         // if
#define FT_ELSEIF 406     // elseif
#define FT_ELSE 407       // else
#define FT_FOR 408        // for
#define FT_WHILE 409      // while
#define FT_BREAK 410      // break
#define FT_CONTINUE 411   // continue
#define FT_FUNC 412       // func
#define FT_CLASS 413      // class
#define FT_TYPE 414       // type
#define FT_ISINSTANCE 415 // isinstance
#define FT_IMPORT 416     // import
#define FT_RETURN 417     // return
#define FT_VOID 418       // void
#define FT_LOGICALAND 419 // and
#define FT_LOGICALOR 420  // or
#define FT_LOGICALNOT 421 // not

// constants
#define FT_INTEGER 501    // integer number
#define FT_REAL 502       // real number
#define FT_STRING 503     // string literal
#define FT_IDENTIFIER 504 // identifier
