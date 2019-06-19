// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Log.h"
#include <stdio.h>

int main(int argc, char **argv) {
  auto log = fastype::LogManager::getLogger("root");
  F_DEBUGF(log, "{} {}", "DEBUGF", 2);
  F_DEBUG(log, "DEBUG");
  return 0;
}
