// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "boost/filesystem.hpp"
#include "catch2/catch.hpp"
#include "fmt/format.h"
#include "spdlog/sinks/basic_file_sink.h"

TEST_CASE("Main", "[Main]") {
  std::string logName = "nerd-test";
  std::time_t timestamp = std::time(nullptr);
  std::tm dt = *std::localtime(&timestamp);
  std::string logFile =
      fmt::format("{}/{}-{:04d}.{:02d}.{:02d}-{:02d}.{:02d}.{:02d}.log", ".",
                  logName, dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday,
                  dt.tm_hour, dt.tm_min, dt.tm_sec);
  spdlog::set_default_logger(spdlog::basic_logger_mt(logName, logFile));
  SECTION("Main Initialize") {
    fmt::print(
        "nerd-test start at:{}\n",
        boost::filesystem::path(boost::filesystem::current_path()).string());
    LOG_INFO(
        "nerd-test start at:{}",
        boost::filesystem::path(boost::filesystem::current_path()).string());
  }
}
