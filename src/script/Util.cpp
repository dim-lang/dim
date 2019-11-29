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
    if (vec[i]) {
      ss << vec[i]->toString();
    } else {
      ss << "null";
    }
    ss << ", ";
  }
  ss << "]";
  return ss.str();
}

} // namespace detail

} // namespace fastype
