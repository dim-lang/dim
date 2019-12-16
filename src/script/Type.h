// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"

namespace fastype {

class Type : public Stringify {
public:
  virtual ~Type() = default;
  static const Type &fromValue(int value);
  static const Type &fromName(const icu::UnicodeString &name);
  virtual int value() const;
  virtual const icu::UnicodeString &name() const;
  virtual std::string nameUTF8() const;
  virtual std::string toString() const;

  // token type
  static const Type T_EOF;         // end of file
  static const Type T_INTEGER;     // integer number
  static const Type T_FLOATING;    // floating number
  static const Type T_OPERATOR;    // operator: + - * / % ++ --
  static const Type T_COMPARATOR;  // comparator: == != < <= > >=
  static const Type T_ASSIGNMENT;  // assignment: =
  static const Type T_BOOLEAN;     // boolean: True False
  static const Type T_IDENTIFIER;  // identifier
  static const Type T_PUNCTUATION; // punctuation
  static const Type T_KEYWORD;     // keyword: let null for if elseif else
                                   // while break continue func class
  static const Type T_STRING;      // string

  // ast type
  static const Type T_PROGRAM;
  static const Type T_STATEMENT_LIST;
  static const Type T_DECLARATION;
  static const Type T_STATEMENT;
  static const Type T_VARIABLE_DECLARATION;
  static const Type T_FUNCTION_DECLARATION;
  static const Type T_CLASS_DECLARATION;
  static const Type T_COMPOUND_STATEMENT;
  static const Type T_ASSIGNMENT_STATEMENT;
  static const Type T_EMPTY_STATEMENT;
  static const Type T_RETURN_STATEMENT;
  static const Type T_BINARY_OP;
  static const Type T_UNARY_OP;
  static const Type T_VARIABLE;
  static const Type T_INTEGER_CONSTANT;
  static const Type T_FLOATING_CONSTANT;
  static const Type T_BOOLEAN_CONSTANT;
  static const Type T_STRING_CONSTANT;

  bool operator==(const Type &t) const;
  bool operator!=(const Type &t) const;
  bool operator<(const Type &t) const;
  bool operator<=(const Type &t) const;
  bool operator>(const Type &t) const;
  bool operator>=(const Type &t) const;
  int compare(const Type &t) const;

private:
  Type(const icu::UnicodeString &name);

  static int TypeId;

  icu::UnicodeString name_;
  int value_;
};

} // namespace fastype
