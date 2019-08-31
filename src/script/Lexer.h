// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "ResourceHandler.h"
#include "script/Token.h"
#include <deque>
#include <regex>

namespace fastype {

class Lexer {
public:
  static const std::string RegexPattern_ =
      "\\s*((//.*)|([0-9]+)|(\"(\\\\\"|\\\\\\\\|\\\\n|[^\"])*\")"
      "|[A-Z_a-z][A-Z_a-z0-9]*|==|!=|<|<=|>|>=|&&|\\|\\||\\p{Punct})?";

  Lexer(void *resource, ResourceHandler resourceHandler);
  virtual ~Lexer();

  virtual Token read();
  virtual Token peek(int i);

private:
  virtual bool fillQueue(int i);
  virtual void readLine();
  virtual std::string toStringLiteral(const std::string &s);

  std::regex pattern_;
  std::deque<Token> queue_;
  bool more_;
  int index_;
  int length_;
  void *data_;
  ResourceHandler resourceHandler_;
};

} // namespace fastype
