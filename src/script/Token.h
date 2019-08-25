// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "exception/ScriptException.h"
#include <regex>
#include <string>

namespace fastype {

class Token {
public:
  static const Token EOF_;
  static const std::string EOL_;

  virtual ~Token() = default;
  virtual const int &lineNumber() const { return lineNumber_; }
  virtual bool isIdentifier() const { return false; }
  virtual bool isNumber() const { return false; }
  virtual bool isString() const { return false; }
  virtual int number() const {
    throw new ScriptException("Token#number not implement");
  }
  virtual std::string text() const { return ""; }

private:
  Token(int line) : lineNumber_(line) {}

  int lineNumber_;
};

} // namespace fastype
