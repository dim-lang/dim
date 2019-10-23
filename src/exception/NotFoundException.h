// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "exception/Exception.h"

namespace fastype {

class NotFoundException : public BaseException {
public:
  NotFoundException(const char *fileName, int lineNumber,
                    const char *functionName, const std::string &message)
      : BaseException(fileName, lineNumber, functionName, message) {}
};

} // namespace fastype
