// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Option.h"
#include "catch2/catch.hpp"

TEST_CASE("Option", "[Option]") {
  SECTION("Option") {
    {
      char *argv[] = {"./dimc", "-h"};
      int argc = 2;
      Option opt(argc, argv);
      REQUIRE(opt.has("help"));
    }
    {
      char *argv[] = {"./dimc", "--version"};
      int argc = 2;
      Option opt(argc, argv);
      REQUIRE(opt.has("version"));
    }
    {
      char *argv[] = {"./dimc", "test/case/parse-1.dim", "test/case/parse-2.dim", "test/case/parse-3.dim"};
      int argc = 4;
      Option opt(argc, argv);
      REQUIRE(opt.has("input-files"));
      std::vector<std::string> inputFileList = opt.get<std::vector<std::string>>("input-files");
      REQUIRE(inputFileList.size() == 3);
      REQUIRE(inputFileList[0] == "test/case/parse-1.dim");
      REQUIRE(inputFileList[1] == "test/case/parse-2.dim");
      REQUIRE(inputFileList[2] == "test/case/parse-3.dim");
    }
    {
      char *argv[] = {"./dimc", "test/case/parse-3.dim", "--output", "parse-3.o", "--codegen", "obj"};
      int argc = 6;
      Option opt(argc, argv);
      REQUIRE(opt.has("input-files"));
      std::vector<std::string> inputFileList = opt.get<std::vector<std::string>>("input-files");
      REQUIRE(inputFileList.size() == 1);
      REQUIRE(inputFileList[0] == "test/case/parse-3.dim");
      REQUIRE(opt.has("output"));
      REQUIRE(opt.get<std::string>("output") == "parse-3.o");
      REQUIRE(opt.has("codegen"));
      REQUIRE(opt.get<std::string>("codegen") == "obj");
    }
    {
      char *argv[] = {"./dimc", "-oparse.o", "-cobj", "-O3", "test/case/parse-3.dim"};
      int argc = 5;
      Option opt(argc, argv);
      REQUIRE(opt.has("input-files"));
      std::vector<std::string> inputFileList = opt.get<std::vector<std::string>>("input-files");
      REQUIRE(inputFileList.size() == 1);
      REQUIRE(inputFileList[0] == "test/case/parse-3.dim");
      REQUIRE(opt.has("output"));
      REQUIRE(opt.get<std::string>("output") == "parse.o");
      REQUIRE(opt.has("codegen"));
      REQUIRE(opt.get<std::string>("codegen") == "obj");
      REQUIRE(opt.has("optimize"));
      REQUIRE(opt.get<int>("optimize") == 3);
    }
  }
}

