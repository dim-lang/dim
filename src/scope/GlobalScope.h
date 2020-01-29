// copyright 2019- <coli-lang>
// apache license version 2.0

#pragma once
#include "Scope.h"

class GlobalScope : public Scope {
public:
  virtual ~GlobalScope() = default;
  static std::shared_ptr<Scope> instance();

private:
  GlobalScope();
};
