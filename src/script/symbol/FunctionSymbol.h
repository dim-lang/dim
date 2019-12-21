// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Symbol.h"

namespace fastype {

class FunctionSymbol : public Symbol {
public:
  FunctionSymbol(const icu::UnicodeString &name,
                 const std::vector<std::shared_ptr<Symbol>> &parameterList,
                 std::shared_ptr<Symbol> type);
  virtual ~FunctionSymbol() = default;
  virtual const icu::UnicodeString &name() const;
  virtual const std::vector<std::shared_ptr<Symbol>> &parameterList() const;
  virtual std::shared_ptr<Symbol> type() const;
  virtual bool equal(std::shared_ptr<Symbol> s) const;
  virtual std::string toString() const;

private:
  icu::UnicodeString name_;
  std::vector<std::shared_ptr<Symbol>> parameterList_;
  std::shared_ptr<Symbol> type_;
};

} // namespace fastype
