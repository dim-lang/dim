// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "SmartPointer.h"
#include "Stringify.h"
#include "exception/ScriptException.h"
#include <regex>
#include <string>

namespace fastype {

class Token : public Stringify {
public:
  enum TokenType {
    TT_EOF,         // end of file
    TT_EOL,         // end of line, '\n' '\r\n'
    TT_KEYWORD,     // key word: let, func, class, null
    TT_OPERATOR,    // operator: + - * / % = == != < <= > >=
    TT_STRING,      // string: "A", "Hello World!\\"
    TT_BOOLEAN,     // boolean: True, False
    TT_INTEGER,     // integer: 1, 2, 3, -10
    TT_FLOATING,    // floating: 0.1, 2e-5, -1.034e+4
    TT_PUNCTUATION, // punctuation: , ; ? : ( ) [ ] { }
    TT_COMMENT,     // comment: one line comment, block comment
    TT_IDENTIFIER,  // variable/function/class identifier
  };

  const static Token T_EOF; // -1

  Token(int lineNumber, Cowstr literal, TokenType type);
  virtual ~Token() = default;

  // compare only token type and literal, exclude the line number
  virtual bool operator==(const Token &t) const;
  virtual bool operator!=(const Token &t) const;

  virtual const int &lineNumber() const;
  virtual const TokenType &type() const;
  // raw literal string
  virtual const Cowstr &literal() const;

  // token type
  virtual bool isEof() const;
  virtual bool isEol() const;
  virtual bool isKeyword() const;
  virtual bool isOperator() const;
  virtual bool isString() const;
  virtual bool isBoolean() const;
  virtual bool isInteger() const;
  virtual bool isFloating() const;
  virtual bool isPunctuation() const;
  virtual bool isComment() const;
  virtual bool isIdentifier() const;

  virtual const Cowstr &text() const;
  virtual int64_t integer() const;
  virtual float floating() const;
  virtual bool boolean() const;

  virtual std::string toString() const = 0;

protected:
  int lineNumber_;
  Cowstr literal_;
  TokenType type_;
};

} // namespace fastype
