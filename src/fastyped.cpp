// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Daemonize.h"
#include "Logging.h"

int main(int argc, char **argv) {
  F_LOGGER("fastyped");
  F_INFO("starting...");

  fastype::Daemonize::daemon();

  return 0;
}
