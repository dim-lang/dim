// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace fastype {

class LineBound {
public:
  LineBound();
  LineBound(int32_t buffer, int32_t byte);
  virtual ~LineBound() = default;

  int32_t buffer() const;
  int32_t setBuffer(int32_t buffer);

  int32_t byte() const;
  int32_t setByte(int32_t byte);

  bool unset() const;
  static const LineBound &undefined();

  std::string toString() const;

private:
  int32_t buffer_;
  int32_t byte_;
};

} // namespace fastype
