// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"

namespace fastype {

class MetaType : public Stringify {
public:
  virtual ~MetaType() = default;
  static const MetaType &fromValue(int value);
  static const MetaType &fromName(const icu::UnicodeString &name);
  virtual int value() const;
  virtual const icu::UnicodeString &name() const;
  virtual std::string nameUTF8() const;
  virtual std::string toString() const;

  // token type
  static const MetaType MT_EOF;         // end of file
  static const MetaType MT_INTEGER;     // integer number
  static const MetaType MT_FLOATING;    // floating number
  static const MetaType MT_OPERATOR;    // operator: + - * / % ++ --
  static const MetaType MT_COMPARATOR;  // comparator: == != < <= > >=
  static const MetaType MT_ASSIGNMENT;  // assignment: =
  static const MetaType MT_BOOLEAN;     // boolean: True False
  static const MetaType MT_IDENTIFIER;  // identifier
  static const MetaType MT_PUNCTUATION; // punctuation
  static const MetaType MT_KEYWORD;     // keyword: let null for if elseif else
                                        // while break continue func class
  static const MetaType MT_STRING;      // string

  // ast type
  static const MetaType MT_PROGRAM;
  static const MetaType MT_STATEMENT_LIST;
  static const MetaType MT_DECLARATION;
  static const MetaType MT_STATEMENT;
  static const MetaType MT_VARIABLE_DECLARATION;
  static const MetaType MT_FUNCTION_DECLARATION;
  static const MetaType MT_CLASS_DECLARATION;
  static const MetaType MT_COMPOUND_STATEMENT;
  static const MetaType MT_ASSIGNMENT_STATEMENT;
  static const MetaType MT_EMPTY_STATEMENT;
  static const MetaType MT_RETURN_STATEMENT;
  static const MetaType MT_BINARY_OP;
  static const MetaType MT_UNARY_OP;
  static const MetaType MT_VARIABLE;
  static const MetaType MT_INTEGER_CONSTANT;
  static const MetaType MT_FLOATING_CONSTANT;
  static const MetaType MT_BOOLEAN_CONSTANT;
  static const MetaType MT_STRING_CONSTANT;

private:
  MetaType(const icu::UnicodeString &name);

  static int TypeId;

  icu::UnicodeString name_;
  int value_;
};

} // namespace fastype
