// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include "script/Type.h"
#include <memory>

namespace fastype {

class Ast : public Stringify {
public:
  /**
   * ast type
   *
   * PROGRAM
   * STATEMENT_LIST
   * DECLARATION
   * STATEMENT
   * VARIABLE_DECLARATION
   * FUNCTION_DECLARATION
   * CLASS_DECLARATION
   * COMPOUND_STATEMENT
   * ASSIGNMENT_STATEMENT
   * EMPTY_STATEMENT
   * RETURN_STATEMENT
   * BINARY_OP
   * UNARY_OP
   * VARIABLE
   * INTEGER_CONSTANT
   * FLOATING_CONSTANT
   * BOOLEAN_CONSTANT
   * STRING_CONSTANT
   */
  static const std::vector<int> &astTypes();

  virtual ~Ast() = default;
  virtual int type() const = 0;
  virtual std::string toString() const = 0;
};

} // namespace fastype
