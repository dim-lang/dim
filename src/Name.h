// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Counter.h"
#include "container/Cowstr.h"

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

namespace detail {

class NameGenerator {
public:
  virtual ~NameGenerator() = default;
  virtual Cowstr from(const Cowstr &hint);

protected:
  Counter counter_;
};

} // namespace detail

class AstGraphNameGenerator : public detail::NameGenerator {
public:
  virtual ~AstGraphNameGenerator() = default;
  virtual Cowstr from(const Cowstr &hint);
};

class SymbolNameGenerator : public detail::NameGenerator {
public:
  virtual ~SymbolNameGenerator() = default;
  virtual Cowstr from(const Cowstr &hint);
};

class IrNameGenerator : public detail::NameGenerator {
public:
  virtual ~IrNameGenerator() = default;
  virtual Cowstr from(const Cowstr &hint);
};
