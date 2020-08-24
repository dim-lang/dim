// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Name.h"

class LabelNamely {
public:
  LabelNamely(const std::string &name);
  virtual ~LabelNamely() = default;
  virtual const Name &name() const;

protected:
  Name name_;
};

class Label {};