// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "SymbolClasses.h"

class TypeSymbolizable {
public:
  TypeSymbolizable(TypeSymbol *typeSymbol = nullptr);
  virtual ~TypeSymbolizable() = default;
  virtual TypeSymbol *&typeSymbol();
  virtual TypeSymbol *typeSymbol() const;
  virtual void resetTypeSymbol();

protected:
  TypeSymbol *typeSymbolizable_;
};
