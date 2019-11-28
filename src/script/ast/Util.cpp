// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/Util.h"
#include <sstream>

namespace fastype {

namespace ast {

std::string AstVectortoString(const std::vector<std::shared_ptr<Ast>> &vec,
                              const std::string &name) {
  std::stringstream ss;
  ss << fmt::format("[ @{} children_#size: ", name);
  ss << vec.size() << ", ";
  for (int i = 0; i < vec.size(); i++) {
    ss << i << ":" << vec[i]->toString() << ", "
  }
  ss << "]";
  return ss.str();
}

} // namespace ast

} // namespace fastype
