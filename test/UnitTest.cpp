// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "boost/filesystem.hpp"
#include "catch2/catch.hpp"
#include "fmt/format.h"
#include "infra/Log.h"
#include "spdlog/sinks/basic_file_sink.h"

struct LogInitializer {
  LogInitializer() {
    std::string logName = "dim-test";
    std::time_t timestamp = std::time(nullptr);
    std::tm dt = *std::localtime(&timestamp);
    std::string logFile =
        fmt::format("{}/{}-{:04d}.{:02d}.{:02d}-{:02d}.{:02d}.{:02d}.log", ".",
                    logName, dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday,
                    dt.tm_hour, dt.tm_min, dt.tm_sec);
    spdlog::set_pattern(
        "%Y-%m-%d %H:%M:%S.%e [%n] %l process-%P thread-%t [%@ %!] %v");
    spdlog::set_default_logger(spdlog::basic_logger_mt(logName, logFile));
  }
};

static LogInitializer initializer;

TEST_CASE("Unit Test", "[Unit Test]") {
  SECTION("Unit Test Initialize") {
    fmt::print(
        "dim-test start at:{}\n",
        boost::filesystem::path(boost::filesystem::current_path()).string());
    LOG_INFO(
        "dim-test start at:{}",
        boost::filesystem::path(boost::filesystem::current_path()).string());
  }
}
