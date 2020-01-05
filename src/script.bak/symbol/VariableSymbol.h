// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Symbol.h"

namespace fastype {

class VariableSymbol : public Symbol {
public:
  VariableSymbol(const icu::UnicodeString &name, std::shared_ptr<Symbol> type);
  virtual ~VariableSymbol() = default;
  virtual const icu::UnicodeString &name() const;
  virtual std::shared_ptr<Symbol> type() const;
  virtual bool equal(std::shared_ptr<Symbol> s) const;
  virtual std::string toString() const;

private:
  icu::UnicodeString name_;
  std::shared_ptr<Symbol> type_;
};

} // namespace fastype
