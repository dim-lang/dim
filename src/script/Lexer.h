// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "ResourceHandler.h"
#include "SmartPointer.h"
#include "script/Token.h"
#include <deque>

namespace fastype {

class Lexer {
public:
  Lexer(void *resource, ResourceHandler resourceHandler);
  virtual ~Lexer();

  virtual Token read();
  virtual Token peek(int i);

private:
  virtual bool fillQueue(int i);
  virtual void readLine();
  virtual std::string toStringLiteral(const std::string &s);

  std::deque<Sptr<Token>> queue_;
  bool more_;
  int index_;
  int length_;
  ResourceHolder resourceHolder_;
};

} // namespace fastype
