// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <vector>

namespace fastype {

class Token : public Stringify {
public:
  // token type
  enum TokenType {
    TT_EOF,         // end of file
    TT_INTEGER,     // integer number
    TT_FLOATING,    // floating number
    TT_OPERATOR,    // operator: + - * / % ++ --
    TT_COMPARATOR,  // comparator: == != < <= > >=
    TT_ASSIGNMENT,  // assignment: =
    TT_BOOLEAN,     // boolean: True False
    TT_IDENTIFIER,  // identifier
    TT_PUNCTUATION, // punctuation
    TT_KEYWORD,     // keyword: let null for if elseif else
                    //          while break continue func class
    TT_STRING,      // string
  };

  static const std::vector<TokenType> &tokenTypes();
  static int tokenTypeValue(TokenType tt);
  static icu::UnicodeString tokenTypeName(TokenType tt);
  static TokenType tokenTypeFromValue(int value);
  static TokenType tokenTypeFromName(const icu::UnicodeString &name);

  // token constants

  // eofs
  const static std::shared_ptr<Token> T_EOF;

  // operators
  const static std::shared_ptr<Token> T_ADD;       // +
  const static std::shared_ptr<Token> T_SUB;       // -
  const static std::shared_ptr<Token> T_MUL;       // *
  const static std::shared_ptr<Token> T_DIV;       // /
  const static std::shared_ptr<Token> T_MOD;       // %
  const static std::shared_ptr<Token> T_NOT;       // !
  const static std::shared_ptr<Token> T_INC;       // ++
  const static std::shared_ptr<Token> T_DEC;       // --
  const static std::shared_ptr<Token> T_ADDASSIGN; // +=
  const static std::shared_ptr<Token> T_SUBASSIGN; // -=
  const static std::shared_ptr<Token> T_MULASSIGN; // *=
  const static std::shared_ptr<Token> T_DIVASSIGN; // /=
  const static std::shared_ptr<Token> T_MODASSIGN; // %=

  // assignment
  const static std::shared_ptr<Token> T_ASSIGNMENT; // =

  // comparator
  const static std::shared_ptr<Token> T_EQ;  // ==
  const static std::shared_ptr<Token> T_NEQ; // !=
  const static std::shared_ptr<Token> T_LT;  // <
  const static std::shared_ptr<Token> T_LE;  // <=
  const static std::shared_ptr<Token> T_GT;  // >
  const static std::shared_ptr<Token> T_GE;  // >=

  // booleans
  const static std::shared_ptr<Token> T_TRUE;  // True
  const static std::shared_ptr<Token> T_FALSE; // False

  // punctuations
  const static std::shared_ptr<Token> T_LP;       // (
  const static std::shared_ptr<Token> T_RP;       // )
  const static std::shared_ptr<Token> T_LBRACKET; // [
  const static std::shared_ptr<Token> T_RBRACKET; // ]
  const static std::shared_ptr<Token> T_LBRACE;   // {
  const static std::shared_ptr<Token> T_RBRACE;   // }
  const static std::shared_ptr<Token> T_COMMA;    // ,
  const static std::shared_ptr<Token> T_SEMI;     // ;
  const static std::shared_ptr<Token> T_QUESTION; // ?
  const static std::shared_ptr<Token> T_COLON;    // :

  // keywords
  const static std::shared_ptr<Token> T_LET;        // let
  const static std::shared_ptr<Token> T_NULL;       // null
  const static std::shared_ptr<Token> T_IF;         // if
  const static std::shared_ptr<Token> T_ELSEIF;     // elseif
  const static std::shared_ptr<Token> T_ELSE;       // else
  const static std::shared_ptr<Token> T_FOR;        // for
  const static std::shared_ptr<Token> T_WHILE;      // while
  const static std::shared_ptr<Token> T_BREAK;      // break
  const static std::shared_ptr<Token> T_CONTINUE;   // continue
  const static std::shared_ptr<Token> T_FUNC;       // func
  const static std::shared_ptr<Token> T_CLASS;      // class
  const static std::shared_ptr<Token> T_TYPE;       // type
  const static std::shared_ptr<Token> T_ISINSTANCE; // isinstance
  const static std::shared_ptr<Token> T_IMPORT;     // import
  const static std::shared_ptr<Token> T_RETURN;     // return
  const static std::shared_ptr<Token> T_VOID;       // void

  static const std::vector<std::shared_ptr<Token>> &eofs();
  static const std::vector<std::shared_ptr<Token>> &operators();
  static const std::vector<std::shared_ptr<Token>> &assignments();
  static const std::vector<std::shared_ptr<Token>> &comparators();
  static const std::vector<std::shared_ptr<Token>> &booleans();
  static const std::vector<std::shared_ptr<Token>> &punctuations();
  static const std::vector<std::shared_ptr<Token>> &keywords();

  Token(TokenType type);
  virtual ~Token();

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
  virtual int64_t integer() const;
  virtual double floating() const;
  virtual bool boolean() const;

  virtual bool equal(const std::shared_ptr<Token> &t) const;

  virtual std::string toString() const;

protected:
  TokenType type_;
  int64_t id_;
};

} // namespace fastype
