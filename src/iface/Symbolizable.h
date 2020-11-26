// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "SymbolClasses.h"

class Symbolizable {
public:
  Symbolizable(Symbol *symbol = nullptr);
  virtual ~Symbolizable() = default;
  virtual Symbol *&symbol();
  virtual Symbol *symbol() const;
  virtual void resetSymbol();

protected:
  Symbol *symbolizable_;
};
