// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include <string>
#include <unordered_set>

namespace fastype {

class TokenType {
public:
  // token type
  const static TokenType TT_EOF;      // end of file
  const static TokenType TT_EOL;      // end of line, '\n' '\r\n'
  const static TokenType TT_KEYWORD;  // key word: let, func, class, null
  const static TokenType TT_OPERATOR; // operator: + - * / % = == != < <= > >=
  const static TokenType TT_STRING;   // string: "A", "Hello World!\\"
  const static TokenType TT_BOOLEAN;  // boolean: True, False
  const static TokenType TT_INTEGER;  // integer: 1, 2, 3, -10
  const static TokenType TT_FLOATING; // floating: 0.1, 2e-5, -1.034e+4
  const static TokenType TT_PUNCTUATION; // punctuation: , ; ? : ( ) [ ] { }
  const static TokenType TT_COMMENT; // comment: one line comment, block comment
  const static TokenType TT_IDENTIFIER; // variable/function/class identifier

  static const std::unordered_set<TokenType> &all();

  bool operator==(const TokenType &t) const;
  bool operator!=(const TokenType &t) const;
  bool operator<(const TokenType &t) const;
  bool operator<=(const TokenType &t) const;
  bool operator>(const TokenType &t) const;
  bool operator>=(const TokenType &t) const;

  std::string name() const;
  int value() const;

  static TokenType fromName(const std::string &name);
  static TokenType fromName(Cowstr name);
  static TokenType fromValue(int value);

private:
  TokenType(int value, const std::string &name);

  int type_;
  int subtype_;
  std::string name_;
};

} // namespace fastype
