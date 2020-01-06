// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "config/Header.h"
#include <string>
#include <vector>

class Filer {
public:
  // read all into one string
  static icu::UnicodeString read(const icu::UnicodeString &fileName,
                                 const char *locale = nullptr,
                                 const char *codepage = "UTF-8");

  // read all to line by line
  static std::vector<icu::UnicodeString>
  readline(const icu::UnicodeString &fileName, const char *locale = nullptr,
           const char *codepage = "UTF-8");

  // write all into one file
  static int write(const icu::UnicodeString &fileName,
                   const icu::UnicodeString &text, const char *locale = nullptr,
                   const char *codepage = "UTF-8");

  // write all lines into one file
  static int writeline(const icu::UnicodeString &fileName,
                       const std::vector<icu::UnicodeString> &lines,
                       const char *locale = nullptr,
                       const char *codepage = "UTF-8");

  // write all into one file
  static int append(const icu::UnicodeString &fileName,
                    const icu::UnicodeString &text,
                    const char *locale = nullptr,
                    const char *codepage = "UTF-8");

  // write all lines into one file
  static int appendline(const icu::UnicodeString &fileName,
                        const std::vector<icu::UnicodeString> &lines,
                        const char *locale = nullptr,
                        const char *codepage = "UTF-8");
};
