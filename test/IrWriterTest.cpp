// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "IrWriter.h"
#include "Ir.h"
#include "IrContext.h"
#include "Log.h"
#include "Scanner.h"
#include "catch2/catch.hpp"

static void testOutput(const std::string &fileName) {
  Scanner scanner;
  scanner.pushBuffer(fileName);
  REQUIRE(scanner.parse() == 0);
  IrContext context(fileName);
  IrTranslateUnit tunit(&context, scanner.translateUnit());
  tunit.codeGen();
  IrLLWriter writer(&context);
  writer.toStdout();
  LOG_INFO("output string ir:{}\n\n{}", fileName, writer.toStringOstream());
  std::pair<std::error_code, std::string> result = writer.toFileOstream();
  std::error_code errcode = result.first;
  LOG_INFO("output file ir:{}, errcode value:{}, category:{}, message:{}",
           fileName, errcode.value(), errcode.category().name(),
           errcode.message());
}

TEST_CASE("IrWriter", "[IrWriter]") {
  SECTION("Output Ir") {
    testOutput("test/case/IrTest1.shp");
    testOutput("test/case/IrTest2.shp");
    testOutput("test/case/IrTest3.shp");
  }
}

