// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include <fmt/format.h>
#include <memory>
#include <string>
#include <vector>

namespace fastype {

namespace detail {

std::string astVectorToString(const std::vector<std::shared_ptr<Ast>> &vec,
                              const std::string &name);

} // namespace detail

} // namespace fastype
