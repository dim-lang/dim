// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Token.h"

namespace fastype {

Token Token::EOF_ = Token(-1);

std::string Token::EOL_ = "\\n";

const int &Token::lineNumber() const { return lineNumber_; }

bool Token::isIdentifier() const { return false; }

bool Token::isNumber() const { return false; }

bool Token::isString() const { return false; }

bool Token::isBoolean() const { return false; }

int Token::number() const {
  throw new ScriptException("Token#number not implement");
}

std::string Token::text() const {
  throw new ScriptException("Token#text not implement");
}

bool Token::boolean() const {
  throw new ScriptException("Token#boolean not implement");
}

Token::Token(int line) : lineNumber_(line) {}

} // namespace fastype
