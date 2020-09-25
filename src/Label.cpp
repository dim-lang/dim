// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Label.h"
#include "Log.h"
#include "fmt/format.h"
#include <algorithm>

Label::Label(const Cowstr &prefix, const Cowstr &delimiter)
    : prefix_(prefix), delimiter_(delimiter) {}

Cowstr Label::encode(Ast *ast) {
  Cowstr name = ast->name();
  Cowstr location = Cowstr(ast->location().str()).replace('-', '_');
  Cowstr result =
      fmt::format("{0}{1}{2}{1}{3}", prefix_, delimiter_, name, location);
  result = fmt::format("{0}{1}{2:x}", result, delimiter_,
                       std::hash<Cowstr>()(result));
  return result;
}

Cowstr Label::decode(const Cowstr &value) {
  LOG_ASSERT(value.length() > prefix_.length() + 3 * delimiter_.length(),
             "value.length {} > prefix_.length {} + 3 * delimiter_.length {}",
             value.length(), prefix_.length(), delimiter_.length());
  Cowstr result = value.subString(prefix_.length() + delimiter_.length());
  LOG_ASSERT(result.find(delimiter_) >= 0,
             "result {} contains delimiter_ {} at: {}", result, delimiter_,
             result.find(delimiter_));
  return result.subString(0, result.find(delimiter_));
}
