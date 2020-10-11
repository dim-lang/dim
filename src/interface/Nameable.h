// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"

class Nameable {
public:
  virtual ~Nameable() = default;
  virtual Cowstr &name() = 0;
  virtual const Cowstr &name() const = 0;
};

class NameableImpl : public Nameable {
public:
  NameableImpl();
  NameableImpl(const Cowstr &name);
  virtual ~NameableImpl() = default;
  virtual Cowstr &name();
  virtual const Cowstr &name() const;

private:
  Cowstr nameableImpl_;
};
