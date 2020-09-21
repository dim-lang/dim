// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "container/Cowstr.h"

class Ast;
class Symbol;

class Nameable {
public:
  Nameable();
  Nameable(const Cowstr &name);
  virtual ~Nameable() = default;
  virtual Cowstr &name();
  virtual const Cowstr &name() const;

private:
  Cowstr name_;
};
