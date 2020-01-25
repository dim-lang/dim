// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "Symbol.h"
#include <memory>
#include <string>

class Scope : public Stringify {
public:
  virtual const std::string &name() const = 0;
  virtual std::shared_ptr<Scope> enclosing() const = 0;
  virtual void define(std::shared_ptr<Symbol> symbol) = 0;
  virtual std::shared_ptr<Symbol> resolve(const std::string &name) = 0;
  virtual std::string toString() const = 0;
};
