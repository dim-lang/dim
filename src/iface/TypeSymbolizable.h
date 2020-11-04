// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

class TypeSymbol;

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
