// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Symbol.h"

namespace fastype {

class BuiltinTypeSymbol : public Symbol {
public:
  BuiltinTypeSymbol(const icu::UnicodeString &name);
  virtual ~BuiltinTypeSymbol() = default;
  virtual const icu::UnicodeString &name() const;
  virtual int type() const;
  virtual std::string toString() const;

private:
  icu::UnicodeString name_;
};

} // namespace fastype
