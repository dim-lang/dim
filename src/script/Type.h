// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"

// token type
// end of file
#define F_TYPE_EOF 1
// integer number
#define F_TYPE_INTEGER 2
// floating number
#define F_TYPE_FLOATING 3
// operator: + - * / % ++ --
#define F_TYPE_OPERATOR 4
// comparator: == != < <= > >=
#define F_TYPE_COMPARATOR 5
// assignment: =
#define F_TYPE_ASSIGNMENT 6
// boolean: True False
#define F_TYPE_BOOLEAN 7
// identifier
#define F_TYPE_IDENTIFIER 8
// punctuation
#define F_TYPE_PUNCTUATION 9
// keyword: let null for if elseif else while break continue func class
#define F_TYPE_KEYWORD 10
// string
#define F_TYPE_STRING 11

// ast type
#define F_TYPE_PROGRAM 101
#define F_TYPE_STATEMENT_LIST 102
#define F_TYPE_DECLARATION 103
#define F_TYPE_STATEMENT 104
#define F_TYPE_VARIABLE_DECLARATION 105
#define F_TYPE_FUNCTION_DECLARATION 106
#define F_TYPE_CLASS_DECLARATION 107
#define F_TYPE_COMPOUND_STATEMENT 108
#define F_TYPE_ASSIGNMENT_STATEMENT 109
#define F_TYPE_EMPTY_STATEMENT 110
#define F_TYPE_RETURN_STATEMENT 111
#define F_TYPE_BINARY_OP 112
#define F_TYPE_UNARY_OP 113
#define F_TYPE_VARIABLE 114
#define F_TYPE_INTEGER_CONSTANT 115
#define F_TYPE_FLOATING_CONSTANT 116
#define F_TYPE_BOOLEAN_CONSTANT 117
#define F_TYPE_STRING_CONSTANT 118

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
  static const Type TP_EOF;         // end of file
  static const Type TP_INTEGER;     // integer number
  static const Type TP_FLOATING;    // floating number
  static const Type TP_OPERATOR;    // operator: + - * / % ++ --
  static const Type TP_COMPARATOR;  // comparator: == != < <= > >=
  static const Type TP_ASSIGNMENT;  // assignment: =
  static const Type TP_BOOLEAN;     // boolean: True False
  static const Type TP_IDENTIFIER;  // identifier
  static const Type TP_PUNCTUATION; // punctuation
  static const Type TP_KEYWORD;     // keyword: let null for if elseif else
                                    // while break continue func class
  static const Type TP_STRING;      // string

  // ast type
  static const Type TP_PROGRAM;
  static const Type TP_STATEMENT_LIST;
  static const Type TP_DECLARATION;
  static const Type TP_STATEMENT;
  static const Type TP_VARIABLE_DECLARATION;
  static const Type TP_FUNCTION_DECLARATION;
  static const Type TP_CLASS_DECLARATION;
  static const Type TP_COMPOUND_STATEMENT;
  static const Type TP_ASSIGNMENT_STATEMENT;
  static const Type TP_EMPTY_STATEMENT;
  static const Type TP_RETURN_STATEMENT;
  static const Type TP_BINARY_OP;
  static const Type TP_UNARY_OP;
  static const Type TP_VARIABLE;
  static const Type TP_INTEGER_CONSTANT;
  static const Type TP_FLOATING_CONSTANT;
  static const Type TP_BOOLEAN_CONSTANT;
  static const Type TP_STRING_CONSTANT;

  bool operator==(const Type &t) const;
  bool operator!=(const Type &t) const;
  bool operator<(const Type &t) const;
  bool operator<=(const Type &t) const;
  bool operator>(const Type &t) const;
  bool operator>=(const Type &t) const;
  int compare(const Type &t) const;

private:
  Type(const icu::UnicodeString &name, int typeId);

  icu::UnicodeString name_;
  int value_;
};

} // namespace fastype
