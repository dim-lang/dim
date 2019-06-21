// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Log.h"
#include "Term.h"
#include <memory>
#include <stdio.h>

int main(int argc, char **argv) {
  auto log = fastype::LogManager::getLogger("main");
  F_DEBUGF(log, "argc: {}", argc);
  for (int i = 0; i < argc; i++) {
    F_DEBUGF(log, "argv[{}]: {}", i, argv[i]);
  }

  if (argc < 2) {
    F_INFO(log, "file name missing!");
    return 3;
  }

  F_DEBUGF(log, "open file: {}", argv[1]);
  // std::shared_ptr<fastype::Term> term = fastype::Term::open(argv[1]);
  // term->show(argv[1]);

  return 0;
}
