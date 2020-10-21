// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Phase.h"
#include "Log.h"

Phase::Phase(const Cowstr &name) : Nameable(name) {}

PhaseManager::PhaseManager(Ast *ast) : ast_(ast) {}

void PhaseManager::add(Phase *phase) {
  LOG_ASSERT(phase, "phase must not null");
  phases_.push_back(phase);
}

void PhaseManager::run() {
  for (int i = 0; i < (int)phases_.size(); i++) {
    phases_[i]->run(ast_);
  }
}
