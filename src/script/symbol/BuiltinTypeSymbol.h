// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Symbol.h"

namespace fastype {

class BuiltinTypeSymbol : public Symbol {
public:
  static const BuiltinTypeSymbol S_INTEGER;
  static const BuiltinTypeSymbol S_FLOATING;

  BuiltinTypeSymbol(const icu::UnicodeString &name);
  virtual ~BuiltinTypeSymbol() = default;
  virtual const icu::UnicodeString &name() const;
  virtual std::shared_ptr<Symbol> type() const;
  virtual bool equal(std::shared_ptr<Symbol> s) const;
  virtual std::string toString() const;

private:
  icu::UnicodeString name_;
};

} // namespace fastype
