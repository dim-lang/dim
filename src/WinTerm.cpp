// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "WinTerm.h"
#include "Logging.h"

namespace fastype {

WinTerm::WinTerm() : Term() { F_DEBUG("Construct"); }

WinTerm::~WinTerm() { F_DEBUG("Destruct"); }

void WinTerm::show(const std::string &fileName) {
  F_DEBUGF("show fileName:{}", fileName);
}

} // namespace fastype
