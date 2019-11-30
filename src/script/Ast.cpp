// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Ast.h"
#include "Logging.h"
#include "exception/NotFoundException.h"

namespace fastype {

Ast ::~Ast() { F_INFO("Destructor {}", toString()); }

std::string Ast::toString() const {
  F_THROW_MSG(NotFoundException, "toString not implement!");
}

} // namespace fastype
