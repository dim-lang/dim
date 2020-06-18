// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Buffer.h"
#include "Log.h"
#include "Parser.h"
#include "Scanner.h"
#include "Semantic.h"
#include "catch2/catch.hpp"

static void testAST(const char *fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  LOG_INFO("dump ast: {} {}", fileName, dumpAst(scanner.translateUnit()));
  SymbolManager smanager;
  Semantic::build(&smanager, scanner.translateUnit());
  Semantic::check(&smanager, scanner.translateUnit());
  LOG_INFO("dump symbol:{} {}", fileName,
           dumpScope(Scope::make_snode(smanager.global, ScopeType::ty_global(),
                                       scanner.translateUnit())));
  std::printf(
      "dump source:%s\n%s", fileName,
      dumpSource(fileName, scanner.translateUnit()->position()).c_str());
  LOG_INFO("dump source:{}\n{}", fileName,
           dumpSource(fileName, scanner.translateUnit()->position()));
}

TEST_CASE("Dump", "[Dump]") {
  SECTION("dump Ast/Symbol/Type/Source") {
    testAST("test/case/Parser1.shp");
    testAST("test/case/Parser2.shp");
  }
}
