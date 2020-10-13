// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Counter.h"

std::unordered_map<Cowstr, Counter *> Counter::instances_ = {};

Counter::Counter(unsigned long long value) : value_(value) {}

unsigned long long Counter::count() { return value_++; }

unsigned long long Counter::total() const { return value_; }

Counter *Counter::get(const Cowstr &name) {
  if (instances_.find(name) == instances_.end()) {
    instances_.insert(std::make_pair(name, new Counter()));
  }
  return instances_[name];
}
