// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Config.h"
#include "Logging.h"
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char **argv) {
  fastype::Config conf(argc, argv);

  if (conf.hasHelp()) {
    std::cout << conf.help() << std::endl;
    return 0;
  }

  if (conf.hasVersion()) {
    std::cout << conf.version() << std::endl;
    return 0;
  }

  F_LOG_INIT("/tmp/fastype/log", "fastyped");
  F_INFO("starting...");
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "looping..." << std::endl;
    F_INFO("looping...");
  }

  return 0;
}
