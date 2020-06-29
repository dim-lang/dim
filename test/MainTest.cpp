// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#define CATCH_CONFIG_MAIN
#include "Log.h"
#include "boost/filesystem.hpp"
#include "catch2/catch.hpp"
#include "fmt/format.h"

TEST_CASE("Main", "[Main]") {
  Log::initialize("shepherd-test");
  SECTION("Main Initialize") {
    fmt::print(
        "shepherd-test start at:{}\n",
        boost::filesystem::path(boost::filesystem::current_path()).string());
    LOG_INFO(
        "shepherd-test start at:{}",
        boost::filesystem::path(boost::filesystem::current_path()).string());
  }
}
