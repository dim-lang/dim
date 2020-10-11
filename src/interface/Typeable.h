// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <memory>

class TypeSymbol;

class Typeable {
public:
  virtual ~Typeable() = default;
  virtual std::shared_ptr<TypeSymbol> type() const = 0;
};

class TypeableImpl : public Typeable {
public:
  TypeableImpl(std::shared_ptr<TypeSymbol> typeableImpl =
                   std::shared_ptr<TypeSymbol>(nullptr));
  virtual ~TypeableImpl() = default;
  virtual std::shared_ptr<TypeSymbol> type() const;

protected:
  std::shared_ptr<TypeSymbol> typeableImpl_;
};
