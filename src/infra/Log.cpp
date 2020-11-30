// Copyright 2019- <rooster-lang>
// Apache License Version 2.0

#include "infra/Log.h"

Exception::Exception(const Cowstr &fileName, int lineNumber,
                     const Cowstr &function, const Cowstr &message)
    : message_(message), fileName_(fileName), lineNumber_(lineNumber),
      function_(function) {}

const Cowstr &Exception::message() const { return message_; }

const Cowstr &Exception::fileName() const { return fileName_; }

const int &Exception::lineNumber() const { return lineNumber_; }

const Cowstr &Exception::function() const { return function_; }

const char *Exception::what() const noexcept { return message_.rawstr(); }
