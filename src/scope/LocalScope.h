// copyright 2019- <coli-lang>
// apache license version 2.0

#pragma once
#include "Scope.h"

class LocalScope : public Scope {
public:
  LocalScope(const std::string &scopeName);
  virtual ~LocalScope() = default;

  virtual std::string name() const;
  virtual std::string toString() const;

private:
  std::string scopeName_;
};
