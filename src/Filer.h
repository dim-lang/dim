// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <string>
#include <vector>

namespace fastype {

class Filer {
public:
  // read all into one string
  static icu::UnicodeString readAll(const icu::UnicodeString &fileName,
                                    const char *locale = nullptr,
                                    const char *codepage = "UTF-8");

  // read all to line by line
  static std::vector<icu::UnicodeString>
  readLines(const icu::UnicodeString &fileName, const char *locale = nullptr,
            const char *codepage = "UTF-8");

  // write all into one file
  static int64_t writeAll(const icu::UnicodeString &fileName,
                          const icu::UnicodeString &text,
                          const char *locale = nullptr,
                          const char *codepage = "UTF-8");

  // write all into one file
  static int64_t appendAll(const icu::UnicodeString &fileName,
                           const icu::UnicodeString &text,
                           const char *locale = nullptr,
                           const char *codepage = "UTF-8");
};

} // namespace fastype
