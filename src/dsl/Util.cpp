// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Util.h"
#include <sstream>

namespace fastype {

namespace detail {

std::string astVectorToString(const std::vector<std::shared_ptr<Ast>> &vec,
                              const std::string &name) {
  std::stringstream ss;
  ss << fmt::format("[ @{} children_#size: ", name);
  ss << vec.size() << ", ";
  for (int i = 0; i < vec.size(); i++) {
    ss << i << ":";
    std::string _1 = vec[i] ? vec[i]->toString() : "null";
    ss << _1 << ", ";
  }
  ss << "]";
  return ss.str();
}

} // namespace detail

} // namespace fastype