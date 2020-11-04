// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Phase.h"
#include "Visitor.h"
#include "infra/Cowstr.h"
#include <vector>

class Ast;

class Dumper : public Phase {
public:
  Dumper();
  virtual ~Dumper();
  virtual void run(Ast *ast);
  virtual std::vector<Cowstr> &dumps();
  virtual const std::vector<Cowstr> &dumps() const;

private:
  VisitorContext *context_;
  VisitorBinder binder_;
  std::vector<Visitor *> visitors_;
  std::vector<Cowstr> dumps_;
};
