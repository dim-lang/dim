// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Phase.h"
#include "Log.h"

Phase::Phase(const Cowstr &name) : Nameable(name) {}

PhaseManager::PhaseManager(const std::vector<Phase *> phases)
    : phases_(phases) {}

void PhaseManager::add(Phase *phase) {
  LOG_ASSERT(phase, "phase must not null");
  phases_.push_back(phase);
}

void PhaseManager::run(Ast *ast) {
  for (int i = 0; i < (int)phases_.size(); i++) {
    LOG_ASSERT(phases_[i], "phases_[{}] must not null", i);
    phases_[i]->run(ast);
  }
}

Phase *PhaseManager::phase(int pos) const { return phases_[pos]; }

Phase *&PhaseManager::phase(int pos) { return phases_[pos]; }
