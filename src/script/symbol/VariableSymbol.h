// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Symbol.h"

namespace fastype {

class VariableSymbol : public Symbol {
public:
  VariableSymbol(const icu::UnicodeString &name,
                 const icu::UnicodeString &type);
  virtual ~VariableSymbol() = default;
  virtual const icu::UnicodeString &name() const;
  virtual const icu::UnicodeString &type() const;
  virtual std::string toString() const;

private:
  icu::UnicodeString name_;
  icu::UnicodeString type_;
};

} // namespace fastype
