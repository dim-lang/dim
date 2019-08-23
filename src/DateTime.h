// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cstdint>

namespace fastype {

class DateTime {
public:
  // @return  current timestamp in seconds
  static int64_t now();

  // @return  current timestamp in milliseconds
  static int64_t millinow();
};

} // namespace fastype
