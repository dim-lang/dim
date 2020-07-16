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
  IrLLWriter llWriter(&context);
  llWriter.toStdout();
  LOG_INFO("output string ir:{}\n\n{}", fileName, llWriter.toStringOstream());
  std::string llFileName = llWriter.toFileOstream();
  LOG_INFO("output file ir:{}, llFileName:{}", fileName, llFileName);

  IrObjWriter objWriter(&context);
  std::string objFileName = objWriter.toFileOstream();
  LOG_INFO("output file ir:{}, objFileName:{}", fileName, objFileName);
}

TEST_CASE("IrWriter", "[IrWriter]") {
  SECTION("Output") {
    testOutput("test/case/IrTest1.shp");
    testOutput("test/case/IrTest2.shp");
    testOutput("test/case/IrTest3.shp");
  }
}

