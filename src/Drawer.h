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
  VisitorContext *context_;
  VisitorBinder binder_;
  std::vector<Visitor *> visitors_;
};
