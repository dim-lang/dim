// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "ResourceHandler.h"
#include "script/Token.h"
#include <deque>
#include <regex>

#define F_PATTERN_NEW_LINE "\\\\n"
#define F_PATTERN_NUMBER "[0-9]+"
#define F_PATTERN_ID "[A-Z_a-z][A-Z_a-z0-9]*"
#define F_PATTERN_EQ "=="
#define F_PATTERN_NE "!="
#define F_PATTERN_LT "<"
#define F_PATTERN_LE "<="
#define F_PATTERN_GT ">"
#define F_PATTERN_GE ">="
#define F_PATTERN_AND "&&"
#define F_PATTERN_OR "\\|\\|"
#define F_PATTERN_WHITESPACES "\\s*"

namespace fastype {

class Lexer {
public:
  static const std::string RegexPattern_ =
      "\\s*((//.*)|([0-9]+)|(\"(\\\\\"|\\\\\\\\|\\\\n|[^\"])*\")"
      "|[A-Z_a-z][A-Z_a-z0-9]*|==|<=|>=|&&|\\|\\||\\p{Punct})?";

  Lexer(void *resource, ResourceHandler *resourceHandler);
  virtual ~Lexer();

  virtual Token read();
  virtual Token peek(int i);

private:
  virtual bool fillQueue(int i);
  virtual void readLine();
  virtual void addToken(int lineNumber, std::sub_match &sm);
  virtual std::string toStringLiteral(const std::string &s);

  std::regex pattern_;
  std::deque<Token> queue_;
  bool more_;
  int index_;
  int length_;
  void *resource_;
  ResourceHandle resourceHandler_;
};

} // namespace fastype

#undef F_PATTERN_NEW_LINE
#undef F_PATTERN_NUMBER
#undef F_PATTERN_ID
#undef F_PATTERN_EQ
#undef F_PATTERN_NE
#undef F_PATTERN_LT
#undef F_PATTERN_LE
#undef F_PATTERN_GT
#undef F_PATTERN_GE
#undef F_PATTERN_AND
#undef F_PATTERN_OR
#undef F_PATTERN_WHITESPACES
