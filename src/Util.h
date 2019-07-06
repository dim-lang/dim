// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#include "boost/type_index.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <utility>

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

#ifndef F_MAX_I32
#define F_MAX_I32(x, y) std::max<int32_t>(x, y)
#endif

#ifndef F_MAX_U32
#define F_MAX_U32(x, y) std::max<uint32_t>(x, y)
#endif

#ifndef F_MAX_I64
#define F_MAX_I64(x, y) std::max<int64_t>(x, y)
#endif

#ifndef F_MAX_U64
#define F_MAX_U64(x, y) std::max<uint64_t>(x, y)
#endif

#ifndef F_MIN_I32
#define F_MIN_I32(x, y) std::min<int32_t>(x, y)
#endif

#ifndef F_MIN_U32
#define F_MIN_U32(x, y) std::min<uint32_t>(x, y)
#endif

#ifndef F_MIN_I64
#define F_MIN_I64(x, y) std::min<int64_t>(x, y)
#endif

#ifndef F_MIN_U64
#define F_MIN_U64(x, y) std::min<uint64_t>(x, y)
#endif

namespace fastype {

/// Date Time

uint64_t nowTimestamp();

std::string nowDatetime();

std::string formatDatetime(uint64_t t, const std::string &fmt);

uint64_t parseDatetime(const std::string &datetime, const std::string &fmt);

/// File IO

} // namespace fastype
