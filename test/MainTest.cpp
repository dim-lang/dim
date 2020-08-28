// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "boost/filesystem.hpp"
#include "catch2/catch.hpp"
#include "fmt/format.h"
#include "spdlog/sinks/basic_file_sink.h"

TEST_CASE("Main", "[Main]") {
  spdlog::set_default_logger(
      spdlog::basic_logger_mt("nerd-test", "nerd-test.log"));
  SECTION("Main Initialize") {
    fmt::print(
        "nerd-test start at:{}\n",
        boost::filesystem::path(boost::filesystem::current_path()).string());
    LOG_INFO(
        "nerd-test start at:{}",
        boost::filesystem::path(boost::filesystem::current_path()).string());
  }
}
