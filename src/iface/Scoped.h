// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

class Scope;

class Scoped {
public:
  Scoped(Scope *scope = nullptr);
  virtual ~Scoped() = default;
  virtual Scope *&scope();
  virtual Scope *scope() const;
  virtual void resetScope();

protected:
  Scope *scoped_;
};
