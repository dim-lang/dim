// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Phase.h"
#include "Visitor.h"
#include "infra/Cowstr.h"

class Ast;

class Drawer : public Phase {
public:
  Drawer(const Cowstr &output = "");
  virtual ~Drawer();
  virtual void run(Ast *ast);
  virtual const Cowstr &fileName() const;
  virtual Cowstr &fileName();

private:
  Cowstr fileName_;
  std::vector<Visitor *> visitors_;
  VisitorContext *context_;
  VisitorBinder binder_;
};
