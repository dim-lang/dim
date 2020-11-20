// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "AstClasses.h"
#include "iface/Nameable.h"
#include <vector>

class Phase : public Nameable {
public:
  Phase(const Cowstr &name);
  virtual ~Phase() = default;
  virtual void run(Ast *ast) = 0;
};

class PhaseManager {
public:
  PhaseManager(const std::vector<Phase *> phases = {});
  virtual ~PhaseManager() = default;

  virtual void add(Phase *phase);
  virtual void run(Ast *ast);

  virtual Phase *phase(int pos) const;
  virtual Phase *&phase(int pos);

private:
  std::vector<Phase *> phases_;
};
