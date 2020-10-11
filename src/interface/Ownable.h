// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <memory>

class Scope;

class Ownable {
public:
  virtual ~Ownable() = default;
  virtual std::shared_ptr<Scope> owner() const = 0;
};

class OwnableImpl : public Ownable {
public:
  OwnableImpl(
      std::shared_ptr<Scope> ownableImpl = std::shared_ptr<Scope>(nullptr));
  virtual ~OwnableImpl() = default;
  virtual std::shared_ptr<Scope> owner() const;

protected:
  std::shared_ptr<Scope> ownableImpl_;
};
