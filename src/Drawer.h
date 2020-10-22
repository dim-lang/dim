// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "Phase.h"
#include "Visitor.h"

class Ast;

class Drawer : public Phase {
public:
  Drawer(const Cowstr &output);
  virtual ~Drawer();
  virtual void run(Ast *ast);

private:
  Cowstr fileName_;
  std::vector<Visitor *> visitors_;
  VisitorContext *context_;
  VisitorBinder binder_;
};