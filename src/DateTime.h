// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <cstdint>

class DateTime {
public:
  // @return  current timestamp in seconds
  static long long now();

  // @return  current timestamp in milliseconds
  static long long nowms();
};
