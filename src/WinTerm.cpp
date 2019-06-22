// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "WinTerm.h"

namespace fastype {

WinTerm::WinTerm() : Term(), log(fastype::LogManager::getLogger("WinTerm")) {
  F_DEBUG(log, "Construct");
}

WinTerm::~WinTerm() { F_DEBUG(log, "Destruct"); }

void WinTerm::show(const std::string &fileName) {
  F_DEBUGF(log, "show fileName:{}", fileName);
}

} // namespace fastype

