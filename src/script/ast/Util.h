// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include <fmt/format.h>
#include <string>
#include <vector>

namespace fastype {

namespace ast {

std::string AstVectortoString(const std::vector<Sptr<Ast>> &vec,
                              const std::string &name);

} // namespace ast

} // namespace fastype
