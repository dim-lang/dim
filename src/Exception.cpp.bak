// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "exception/Exception.h"

Exception::Exception(const char *fileName, int lineNumber,
                     const char *functionName, const std::string &message)
    : message_(message), fileName_(fileName), lineNumber_(lineNumber),
      functionName_(functionName) {}

const std::string &Exception::message() const { return message_; }

const std::string &Exception::fileName() const { return fileName_; }

const int &Exception::lineNumber() const { return lineNumber_; }

const std::string &Exception::functionName() const { return functionName_; }

std::string Exception::toString() const {
  return fmt::format("[{}:{}] {} - {}", fileName_, lineNumber_, functionName_,
                     message_);
}
