// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "boost/filesystem.hpp"
#include "catch2/catch.hpp"
#include "fmt/format.h"

TEST_CASE("Main", "[Main]") {
  Log::initialize("nerd-test");
  SECTION("Main Initialize") {
    fmt::print(
        "nerd-test start at:{}\n",
        boost::filesystem::path(boost::filesystem::current_path()).string());
    LOG_INFO(
        "nerd-test start at:{}",
        boost::filesystem::path(boost::filesystem::current_path()).string());
  }
}
