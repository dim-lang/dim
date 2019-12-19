// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include "script/Type.h"
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <vector>

namespace fastype {

class Token : public Stringify {
public:
  /**
   * token type
   * TP_EOF,
   * TP_INTEGER,
   * TP_FLOATING,
   * TP_OPERATOR,
   * TP_COMPARATOR,
   * TP_ASSIGNMENT,
   * TP_BOOLEAN,
   * TP_IDENTIFIER,
   * TP_PUNCTUATION,
   * TP_KEYWORD,
   * TP_STRING,
   */
  static const std::vector<Type> &tokenTypes();

  // token constants

  // eofs
  static const std::shared_ptr<Token> T_EOF;

  // operators
  static const std::shared_ptr<Token> T_ADD;       // +
  static const std::shared_ptr<Token> T_SUB;       // -
  static const std::shared_ptr<Token> T_MUL;       // *
  static const std::shared_ptr<Token> T_DIV;       // /
  static const std::shared_ptr<Token> T_MOD;       // %
  static const std::shared_ptr<Token> T_NOT;       // !
  static const std::shared_ptr<Token> T_INC;       // ++
  static const std::shared_ptr<Token> T_DEC;       // --
  static const std::shared_ptr<Token> T_ADDASSIGN; // +=
  static const std::shared_ptr<Token> T_SUBASSIGN; // -=
  static const std::shared_ptr<Token> T_MULASSIGN; // *=
  static const std::shared_ptr<Token> T_DIVASSIGN; // /=
  static const std::shared_ptr<Token> T_MODASSIGN; // %=

  // assignment
  static const std::shared_ptr<Token> T_ASSIGNMENT; // =

  // comparator
  static const std::shared_ptr<Token> T_EQ;  // ==
  static const std::shared_ptr<Token> T_NEQ; // !=
  static const std::shared_ptr<Token> T_LT;  // <
  static const std::shared_ptr<Token> T_LE;  // <=
  static const std::shared_ptr<Token> T_GT;  // >
  static const std::shared_ptr<Token> T_GE;  // >=

  // booleans
  static const std::shared_ptr<Token> T_TRUE;  // True
  static const std::shared_ptr<Token> T_FALSE; // False

  // punctuations
  static const std::shared_ptr<Token> T_LP;       // (
  static const std::shared_ptr<Token> T_RP;       // )
  static const std::shared_ptr<Token> T_LBRACKET; // [
  static const std::shared_ptr<Token> T_RBRACKET; // ]
  static const std::shared_ptr<Token> T_LBRACE;   // {
  static const std::shared_ptr<Token> T_RBRACE;   // }
  static const std::shared_ptr<Token> T_COMMA;    // ,
  static const std::shared_ptr<Token> T_SEMI;     // ;
  static const std::shared_ptr<Token> T_QUESTION; // ?
  static const std::shared_ptr<Token> T_COLON;    // :

  // keywords
  static const std::shared_ptr<Token> T_LET;        // let
  static const std::shared_ptr<Token> T_NULL;       // null
  static const std::shared_ptr<Token> T_IF;         // if
  static const std::shared_ptr<Token> T_ELSEIF;     // elseif
  static const std::shared_ptr<Token> T_ELSE;       // else
  static const std::shared_ptr<Token> T_FOR;        // for
  static const std::shared_ptr<Token> T_WHILE;      // while
  static const std::shared_ptr<Token> T_BREAK;      // break
  static const std::shared_ptr<Token> T_CONTINUE;   // continue
  static const std::shared_ptr<Token> T_FUNC;       // func
  static const std::shared_ptr<Token> T_CLASS;      // class
  static const std::shared_ptr<Token> T_TYPE;       // type
  static const std::shared_ptr<Token> T_ISINSTANCE; // isinstance
  static const std::shared_ptr<Token> T_IMPORT;     // import
  static const std::shared_ptr<Token> T_RETURN;     // return
  static const std::shared_ptr<Token> T_VOID;       // void

  static const std::vector<std::shared_ptr<Token>> &eofs();
  static const std::vector<std::shared_ptr<Token>> &operators();
  static const std::vector<std::shared_ptr<Token>> &assignments();
  static const std::vector<std::shared_ptr<Token>> &comparators();
  static const std::vector<std::shared_ptr<Token>> &booleans();
  static const std::vector<std::shared_ptr<Token>> &punctuations();
  static const std::vector<std::shared_ptr<Token>> &keywords();

  Token(Type type);
  virtual ~Token();

  virtual const Type &type() const;

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
  Type type_;
};

} // namespace fastype
