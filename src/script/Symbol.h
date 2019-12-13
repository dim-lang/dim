// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include <memory>

namespace fastype {

class Symbol : public Stringify {
public:
  const static icu::UnicodeString T_NONE;
  const static icu::UnicodeString T_BUILTIN;
  const static icu::UnicodeString T_VARIABLE;

  const static std::shared_ptr<Symbol> S_NIL;
  const static std::shared_ptr<Symbol> S_INTEGER;
  const static std::shared_ptr<Symbol> S_FLOATING;

  virtual ~Symbol() = default;
  virtual const icu::UnicodeString &name() const = 0;
  virtual const icu::UnicodeString &type() const = 0;
  virtual std::string toString() const = 0;
};

} // namespace fastype
