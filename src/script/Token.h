// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Stringify.h"
#include <string>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class Token : public Stringify {
public:
  // token type
  enum TokenType {
    TT_EOF = 1,         // end of file
    TT_INTEGER = 2,     // integer number
    TT_FLOATING = 3,    // floating number
    TT_OPERATOR = 4,    // operator: + - * / % ++ --
    TT_COMPARATOR = 5,  // comparator: == != < <= > >=
    TT_ASSIGNMENT = 6,  // assignment: =
    TT_BOOLEAN = 7,     // boolean: True False
    TT_IDENTIFIER = 8,  // identifier
    TT_PUNCTUATION = 9, // punctuation
    TT_KEYWORD = 10,    // keyword: let null for if elseif else
                        //          while break continue func class
    TT_STRING = 11,     // string
  };

  static const std::vector<TokenType> &tokenTypes();
  static int tokenTypeValue(TokenType tt);
  static std::string tokenTypeName(TokenType tt);
  static TokenType tokenTypeFromValue(int value);
  static TokenType tokenTypeFromName(const std::string &name);

  // token constants

  // eofs
  const static Sptr<Token> T_EOF;

  // operators
  const static Sptr<Token> T_ADD;       // +
  const static Sptr<Token> T_SUB;       // -
  const static Sptr<Token> T_MUL;       // *
  const static Sptr<Token> T_DIV;       // /
  const static Sptr<Token> T_MOD;       // %
  const static Sptr<Token> T_NOT;       // !
  const static Sptr<Token> T_INC;       // ++
  const static Sptr<Token> T_DEC;       // --
  const static Sptr<Token> T_ADDASSIGN; // +=
  const static Sptr<Token> T_SUBASSIGN; // -=
  const static Sptr<Token> T_MULASSIGN; // *=
  const static Sptr<Token> T_DIVASSIGN; // /=
  const static Sptr<Token> T_MODASSIGN; // %=

  // assignment
  const static Sptr<Token> T_ASSIGNMENT; // =

  // comparator
  const static Sptr<Token> T_EQ;  // ==
  const static Sptr<Token> T_NEQ; // !=
  const static Sptr<Token> T_LT;  // <
  const static Sptr<Token> T_LE;  // <=
  const static Sptr<Token> T_GT;  // >
  const static Sptr<Token> T_GE;  // >=

  // booleans
  const static Sptr<Token> T_TRUE;  // True
  const static Sptr<Token> T_FALSE; // False

  // punctuations
  const static Sptr<Token> T_LP;       // (
  const static Sptr<Token> T_RP;       // )
  const static Sptr<Token> T_LBRACKET; // [
  const static Sptr<Token> T_RBRACKET; // ]
  const static Sptr<Token> T_LBRACE;   // {
  const static Sptr<Token> T_RBRACE;   // }
  const static Sptr<Token> T_COMMA;    // ,
  const static Sptr<Token> T_SEMI;     // ;
  const static Sptr<Token> T_QUESTION; // ?
  const static Sptr<Token> T_COLON;    // :

  // keywords
  const static Sptr<Token> T_LET;        // let
  const static Sptr<Token> T_NULL;       // null
  const static Sptr<Token> T_IF;         // if
  const static Sptr<Token> T_ELSEIF;     // elseif
  const static Sptr<Token> T_ELSE;       // else
  const static Sptr<Token> T_FOR;        // for
  const static Sptr<Token> T_WHILE;      // while
  const static Sptr<Token> T_BREAK;      // break
  const static Sptr<Token> T_CONTINUE;   // continue
  const static Sptr<Token> T_FUNC;       // func
  const static Sptr<Token> T_CLASS;      // class
  const static Sptr<Token> T_TYPE;       // type
  const static Sptr<Token> T_ISINSTANCE; // isinstance
  const static Sptr<Token> T_IMPORT;     // import
  const static Sptr<Token> T_RETURN;     // return
  const static Sptr<Token> T_VOID;       // void

  static const std::vector<Sptr<Token>> eofs();
  static const std::vector<Sptr<Token>> operators();
  static const std::vector<Sptr<Token>> assignments();
  static const std::vector<Sptr<Token>> comparators();
  static const std::vector<Sptr<Token>> booleans();
  static const std::vector<Sptr<Token>> punctuations();
  static const std::vector<Sptr<Token>> keywords();

  Token(TokenType type);
  virtual ~Token() = default;

  virtual const TokenType &type() const;
  virtual long long id() const;

  // token type
  virtual bool isEof() const;
  virtual bool isInteger() const;
  virtual bool isFloating() const;
  virtual bool isOperator() const;
  virtual bool isComparator() const;
  virtual bool isAssignment() const;
  virtual bool isBoolean() const;
  virtual bool isIdentifier() const;
  virtual bool isPunctuation() const;
  virtual bool isKeyword() const;
  virtual bool isString() const;

  virtual icu::UnicodeString literal() const;
  virtual long long integer() const;
  virtual double floating() const;
  virtual bool boolean() const;

  virtual bool equal(const Sptr<Token> t) const;

  virtual std::string toString() const;

protected:
  TokenType type_;
  long long id_;
};

} // namespace fastype
