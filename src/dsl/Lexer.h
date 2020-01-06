// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include "dsl/Token.h"
#include <deque>
#include <memory>

namespace dsl {

class Lexer : public Stringify {
public:
  Lexer();
  Lexer(const icu::UnicodeString &text);
  Lexer(const std::vector<icu::UnicodeString> &textList);
  virtual ~Lexer();

  void append(const icu::UnicodeString &text);
  void append(const std::vector<icu::UnicodeString> &textList);
  std::shared_ptr<Token> read();
  std::shared_ptr<Token> peek(int pos);
  virtual std::string toString() const;

private:
  void parse();
  bool hasMore();

  void parseWhitespace();
  void parseConstToken(std::shared_ptr<Token> t, int count = 1);
  void parseNumber();
  void parseComment();
  void parseString();
  bool parseIdentifier();
  bool parseKeyword();

  int pos_;
  icu::UnicodeString text_;
  std::deque<icu::UnicodeString> textList_;
  std::deque<std::shared_ptr<Token>> queue_;
};

} // namespace dsl
