// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Stringify.h"
#include "script/Token.h"
#include <deque>
#include <unicode/numfmt.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class Lexer : public Stringify {
public:
  Lexer(const icu::UnicodeString &text);
  virtual ~Lexer();
  Sptr<Token> read();
  Sptr<Token> peek(int pos);
  virtual std::string toString() const;

private:
  void readImpl();

  int pos_;
  icu::UnicodeString text_;
  icu::UnicodeString currentChar_;
  std::deque<Sptr<Token>> queue_;
};

} // namespace fastype
