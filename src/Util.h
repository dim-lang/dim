// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#include "boost/type_index.hpp"
#include <cstdint>
#include <cstdlib>
#include <string>

#ifndef F_STATIC_BLOCK_BEGIN
#define F_STATIC_BLOCK_BEGIN(name)                                             \
  class FastypeStaticBlock##name {                                             \
  public:                                                                      \
    FastypeStaticBlock##name() {
#endif

#ifndef F_STATIC_BLOCK_END
#define F_STATIC_BLOCK_END(name)                                               \
  }                                                                            \
  }                                                                            \
  FastypeStaticBlock##name;
#endif

namespace fastype {

/// Date Time

uint64_t nowTimestamp();

std::string nowDatetime();

std::string formatDatetime(uint64_t t, const std::string &fmt);

uint64_t parseDatetime(const std::string &datetime, const std::string &fmt);

/// File IO

} // namespace fastype
