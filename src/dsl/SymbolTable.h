// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Hasher.h"
#include "Logging.h"
#include "Stringify.h"
#include "script/Symbol.h"
#include "script/Type.h"
#include <memory>
#include <unordered_map>

namespace fastype {

class SymbolTable : public Stringify {
public:
  SymbolTable(const icu::UnicodeString &name, int level);
  virtual ~SymbolTable() = default;
  virtual void insert(std::shared_ptr<Symbol> symbol);
  virtual std::shared_ptr<Symbol> lookup(const icu::UnicodeString &name) const;
  virtual int level() const;
  virtual std::string toString() const;

private:
  std::unordered_map<icu::UnicodeString, std::shared_ptr<Symbol>> table_;
  int level_;
};

} // namespace fastype
