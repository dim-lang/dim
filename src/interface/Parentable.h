// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

class Ast;

class Parentable {
public:
  virtual ~Parentable() = default;
  virtual Ast *parent() const = 0;
};

class ParentableImpl : public Parentable {
public:
  ParentableImpl(Ast *parentableImpl = nullptr);
  virtual ~ParentableImpl() = default;
  virtual Ast *&parent();
  virtual Ast *parent() const;

protected:
  Ast *parentableImpl_;
};
