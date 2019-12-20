// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include "script/Type.h"
#include <memory>

namespace fastype {

class Symbol : public Stringify {
public:
  /**
   * symbol type
   */
  static const std::vector<int> &symbolTypes();

  virtual ~Symbol() = default;
  virtual int type() const = 0;
  virtual std::string toString() const = 0;
};

} // namespace fastype
