// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "WinTerm.h"
#include "Logging.h"
#include <string>

namespace fastype {

WinTerm::WinTerm() : Term() { F_INFO_MSG("Construct"); }

WinTerm::~WinTerm() { F_INFO_MSG("Destruct"); }

void WinTerm::show(const icu::UnicodeString &fileName) {
  std::string _1;
  F_INFO("fileName:{}", fileName.toUTF8String(_1));
}

} // namespace fastype
