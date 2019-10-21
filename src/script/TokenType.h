// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include <string>
#include <unicode/unistr.h>
#include <unicode/ustring.h>
#include <vector>

namespace fastype {

class TokenType {
public:
  // token type
  const static TokenType TT_EOF;        // end of file
  const static TokenType TT_INTEGER;    // integer
  const static TokenType TT_OPERATOR;   // operator: + - * / %
  const static TokenType TT_COMPARATOR; // comparator: == != < <= > >=
  const static TokenType TT_ASSIGNMENT; // assignment: =
  const static TokenType TT_BOOLEAN;    // boolean: True False

  bool operator==(const TokenType &t) const;
  bool operator!=(const TokenType &t) const;
  bool operator<(const TokenType &t) const;
  bool operator<=(const TokenType &t) const;
  bool operator>(const TokenType &t) const;
  bool operator>=(const TokenType &t) const;
  int compare(const TokenType &t) const;

  const icu::UnicodeString &name() const;
  int value() const;

  static TokenType fromName(const icu::UnicodeString &name);
  static TokenType fromValue(int value);
  static const std::vector<TokenType> &types();

private:
  TokenType(int value, const icu::UnicodeString &name);

  int value_;
  icu::UnicodeString name_;
};

} // namespace fastype
