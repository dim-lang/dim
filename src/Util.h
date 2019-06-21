// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#include "boost/type_index.hpp"
#include <cstdint>
#include <cstdlib>
#include <string>

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

#ifndef F_CLASS_NAME
#define F_CLASS_NAME                                                           \
  boost::typeindex::type_id_with_cvr<decltype(*this)>().pretty_name()
#endif

#ifndef F_FUNCTION_NAME
#define F_FUNCTION_NAME __FUNCTION__
#endif

namespace fastype {

/// Date Time

uint64_t nowTimestamp();

std::string nowDatetime();

std::string formatDatetime(uint64_t t, const std::string &fmt);

uint64_t parseDatetime(const std::string &datetime, const std::string &fmt);

/// File IO

} // namespace fastype
