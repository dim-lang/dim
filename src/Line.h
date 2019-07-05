// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Buffer.h"
#include "File.h"
#include "Log.h"
#include "unicode/ustring.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

namespace fastype {

class File;

class Line {
public:
  virtual ~Line() = default;
  Line(const Line &) = default;
  Line &operator=(const Line &) = default;

private:
  Line();

  std::shared_ptr<File> file;

  int32_t startBuffer;
  int32_t startByte;

  int32_t endBuffer;
  int32_t endByte;

  friend class File;
};

} // namespace fastype
