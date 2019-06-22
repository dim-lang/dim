// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Log.h"
#include "Term.h"
#include "config.h"
#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <string>

int main(int argc, char **argv) {
  auto log = fastype::LogManager::getLogger("main");
  F_DEBUGF(log, "argc: {}", argc);
  for (int i = 0; i < argc; i++) {
    F_DEBUGF(log, "argv[{}]: {}", i, argv[i]);
  }

  std::shared_ptr<fastype::Term> term = fastype::Term::open("NCursesTerm");
  term->show("NCursesTerm");

  return 0;
}

