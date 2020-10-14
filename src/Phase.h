// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Name.h"
#include <vector>

class Ast;
class Scanner;

class Phase : public NameableImpl {
public:
  Phase(const Cowstr &name);
  virtual ~Phase() = default;
  virtual void run(Ast *ast) = 0;
};

class PhaseManager {
public:
  PhaseManager(Ast *ast);
  virtual ~PhaseManager() = default;
  virtual void add(Phase *phase);
  virtual void run();
  virtual Cowstr str() const;

private:
  Ast *ast_;
  std::vector<Phase *> phases_;
};
