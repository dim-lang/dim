// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include "config/Header.h"
#include "script/Type.h"
#include <memory>

namespace fastype {

class Symbol : public Stringify {
public:
  /**
   * symbol type
   */
  static const std::vector<int> &symbolTypes();
  static const std::shared_ptr<Symbol> S_NIL;

  virtual ~Symbol() = default;
  virtual const icu::UnicodeString &name() const const = 0;
  virtual std::shared_ptr<Symbol> type() const = 0;
  virtual bool equal(std::shared_ptr<Symbol> s) const = 0;
  virtual std::string toString() const = 0;
};

} // namespace fastype
