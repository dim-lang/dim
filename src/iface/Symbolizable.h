// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

class Symbol;

class Symbolizable {
public:
  Symbolizable(Symbol *symbol = nullptr);
  virtual ~Symbolizable() = default;
  virtual Symbol *&symbol();
  virtual Symbol *symbol() const;

protected:
  Symbol *symbolizable_;
};
