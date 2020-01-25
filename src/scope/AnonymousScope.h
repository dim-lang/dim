// copyright 2019- <coli-lang>
// apache license version 2.0

#pragma once
#include "Scope.h"

class AnonymousScope : public Scope {
public:
  AnonymousScope(const std::string &scopeName);
  virtual ~AnonymousScope() = default;

  virtual std::string name() const;
  virtual std::string toString() const;

private:
  std::string scopeName_;
};
