// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Stringify.h"
#include "script/Token.h"
#include <deque>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class Lexer : public Stringify {
public:
  Lexer(const icu::UnicodeString &text);
  virtual ~Lexer();

  virtual Sptr<Token> read();
  virtual Sptr<Token> peek(int i);
  virtual std::string toString() const;

private:
  virtual bool fillQueue(int i);

  std::deque<Sptr<Token>> queue_;
  bool more_;
  int pos_;
  icu::UnicodeString text_;
};

} // namespace fastype
