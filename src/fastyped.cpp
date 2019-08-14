// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Daemonize.h"
#include "Logging.h"
#include <chrono>
#include <thread>

int main(int argc, char **argv) {
  fastype::Daemonize::daemon();

  F_LOG_INIT("fastyped");
  F_INFO("starting...");
  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    F_INFO("looping...");
  }

  return 0;
}
