// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IdToken.h"
#include "fmt/format.h"

namespace fastype {

bool IdToken::isIdentifier() const { return true; }

std::string IdToken::text() const { return id_; }

std::string IdToken::toString() const {
  return fmt::format("[ @IdToken lineNumber_:{} id_:{} ]", lineNumber_, id_);
}

IdToken::IdToken(int line, const std::string &id) : Token(line), id_(id) {}

} // namespace fastype
