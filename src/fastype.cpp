// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Log.h"
#include <stdio.h>

int main(int argc, char **argv) {
  auto log = fastype::LoggerManager::getLogger("root");
  F_INFOF(log, "{} {}", "INFOF", 2);
  F_INFO(log, "INFO");
  return 0;
}
