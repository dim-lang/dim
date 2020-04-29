// Copyright 2019- <rooster-lang>
// Apache License Version 2.0

#pragma once
#include <cstdint>

class DateTime {
public:
  // @return  current timestamp in seconds
  static int64_t now();

  // @return  current timestamp in milliseconds
  static int64_t millinow();
};
