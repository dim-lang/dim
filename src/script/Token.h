// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "exception/ScriptException.h"
#include <regex>
#include <string>

namespace fastype {

class Token : public Stringify {
public:
  static const std::shared_ptr<Token> EOF_;
  static const std::string EOL_;

  Token(int line);
  virtual ~Token() = default;

  virtual const int &lineNumber() const;
  virtual bool isIdentifier() const;
  // primitive type: number, string, boolean
  virtual bool isNumber() const;
  virtual bool isString() const;
  virtual bool isBoolean() const;
  virtual int number() const;
  virtual std::string text() const;
  virtual bool boolean() const;

  virtual std::string toString() const = 0;

protected:
  int lineNumber_;
};

} // namespace fastype
