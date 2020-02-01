// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Option.h"
#include "Parser.tab.hpp"
#include "Scope.h"
#include "Symbol.h"
#include "Token.h"
#include "Visitor.h"
#include "config/Header.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  Log::initialize(".", "coli");
  Option conf(argc, argv);
  LOG_INFO("starting, argc:{} argv[0]:{}", argc, argv[0]);

  if (conf.hasHelp()) {
    std::cout << conf.help() << std::endl;
    return 0;
  }

  if (conf.hasVersion()) {
    std::cout << conf.version() << std::endl;
    return 0;
  }

  if (conf.hasFileNames()) {
    std::vector<std::string> fileNameList = conf.fileNames();
    for (int i = 0; i < (int)fileNameList.size(); i++) {
      std::string fileName = fileNameList[i];
      std::string moduleName = tokenFileToModule(fileName);
      LOG_INFO("fileName:{} moduleName:{}", fileName, moduleName);
      if (!tokenImportFile(moduleName)) {
        LOG_CHECK(false, "tokenImportFile {} fail", moduleName);
        return 0;
      }
      LOG_INFO("{} starting...", moduleName);
      int yp = yyparse();
      LOG_INFO("{} ending ...", moduleName);
      LOG_CHECK(yp == 0, "yyparse fail:{}", yp);
      ProgramVisitor::instance()->visit(AstProgram::instance());
    }
  }

  return 0;
}
