// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#ifndef F_STATIC_BEGIN
#define F_STATIC_BEGIN(name)                                                   \
  class FastypeStaticBlock##name {                                             \
  public:                                                                      \
    FastypeStaticBlock##name() {
#endif

#ifndef F_STATIC_END
#define F_STATIC_END(name)                                                     \
  }                                                                            \
  }                                                                            \
  FastypeStaticBlock##name;
#endif

#include <cstdint>
#include <cstdlib>
#include <string>

namespace fastype {

/// Date Time

uint64_t nowTimestamp();

std::string nowDatetime();

std::string formatDatetime(uint64_t t, const std::string &fmt);

uint64_t parseDatetime(const std::string &datetime, const std::string &fmt);

/// File IO

} // namespace fastype
