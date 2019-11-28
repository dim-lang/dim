// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "script/Token.h"
#include <deque>
#include <memory>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class Lexer : public Stringify {
public:
  Lexer(const icu::UnicodeString &text);
  virtual ~Lexer();
  std::shared_ptr<Token> read();
  std::shared_ptr<Token> peek(int pos);
  virtual std::string toString() const;

private:
  void readImpl();

  int pos_;
  icu::UnicodeString text_;
  icu::UnicodeString currentChar_;
  std::deque<std::shared_ptr<Token>> queue_;
};

} // namespace fastype
