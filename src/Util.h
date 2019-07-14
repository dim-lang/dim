// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#include "Stringify.h"
#include "boost/type_index.hpp"
#include "fmt/format.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <utility>

namespace fastype {
namespace detail {

// location for file name, line number, function name
class Location : public Stringify {
public:
  Location(const char *fileName, int lineNumber, const char *functionName)
      : fileName_(fileName), lineNumber_(lineNumber),
        functionName_(functionName) {}
  virtual ~Location() = default;

  inline const std::string &fileName() const { return fileName_; }
  inline int lineNumber() const { return lineNumber_; }
  inline const std::string &functionName() const { return functionName_; }
  virtual std::string toString() const {
    return fmt::format("{}:{} {}", fileName_, lineNumber_, functionName_);
  };

  std::string fileName_;
  int lineNumber_;
  std::string functionName_;
};

} // namespace detail
} // namespace fastype

#ifndef LOCATION_ANO
#define LOCATION_ANO fastype::detail::Location(__FILE__, __LINE__, __FUNCTION__)
#endif

#ifndef LOCATION_VAR
#define LOCATION_VAR(x)                                                        \
  fastype::detail::Location x(__FILE__, __LINE__, __FUNCTION__)
#endif

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

#ifndef F_THROW_EX
#define F_THROW_EX(e, msg)                                                     \
  do {                                                                         \
    std::string formatMessage =                                                \
        fmt::format("{}:{} {} {}", __FILE__, __LINE__, __FUNCTION__, msg);     \
    throw new e(formatMessage);                                                \
  } while (0)
#endif

#ifndef F_THROW_EXF
#define F_THROW_EXF(e, fmt, ...)                                               \
  do {                                                                         \
    std::string metaMessage = fmt::format(fmt, __VA_ARGS__);                   \
    F_THROW_EX(e, metaMessage);                                                \
  } while (0)
#endif
