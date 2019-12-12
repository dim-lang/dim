// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Hasher.h"
#include "Stringify.h"
#include "config/Header.h"
#include "script/Symbol.h"
#include <memory>
#include <unordered_map>

namespace fastype {

class SymbolTable : public Stringify {
public:
  SymbolTable();
  virtual ~SymbolTable() = default;
  virtual void define(std::shared_ptr<Symbol> symbol);
  virtual std::shared_ptr<Symbol> lookup(const icu::UnicodeString &name) const;

private:
  std::unordered_map<icu::UnicodeString, std::shared_ptr<Symbol>> symbols_;
};

} // namespace fastype
